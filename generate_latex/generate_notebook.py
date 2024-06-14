"""
Copyright (c) 2024 kyomi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

from os import devnull
import subprocess
from pathlib import Path
from typing import DefaultDict
from re import sub
from rich.logging import RichHandler
import logging


SectionsDict = dict[str, list[Path]]

names = {
    "dp": "DP",
    "ds": "Estruturas de Dados",
    "dfs": "DFS",
    "bfs": "BFS",
    "graph": "Grafos",
    "combinations": "Combinações",
}

logging.basicConfig(
    level="DEBUG",
    format="%(message)s",
    datefmt="[%X]",
    handlers=[RichHandler(rich_tracebacks=True, omit_repeated_times=False)],
)
log = logging.getLogger(__name__)


def normalize_name(name: str) -> str:
    """Normalize the name of the section or code.

    Arguments
    ---------
    name: :class:`str`
        The name to normalize.

    Returns
    -------
    :class:`str`
        The normalized name.
    """
    if name in names:
        log.debug(f"Normalizing '{name}' to '{names[name]}'.")
        return names[name]

    log.debug(f"Could not normalize '{name}'. Using default normalization.")
    return " ".join([x.capitalize() for x in name.split("_")])


def get_template(name: str = "notebook.tex") -> str:
    """Get the template file as a string.

    Arguments
    ---------
    name: :class:`str`
        The name of the template file. Default is `notebook.tex`.

    Returns
    -------
    :class:`str`
        The content of the template file.
    """
    return Path("templates", name).read_text(encoding="utf-8")


def get_sections(path: str = "codes") -> SectionsDict:
    """Get the sections of the codes.

    Arguments
    ---------
    path: :class:`str`
        The path to the codes. Default is `codes`.

    Returns
    -------
    dict[str, list[:class:`pathlib.Path`]]
        A dictionary with the sections as keys and a list of paths as
        values.
    """
    sections: DefaultDict[str, list[Path]] = DefaultDict(list)

    codes = Path(path)
    folders = list(codes.iterdir())

    log.debug("------------------------------")
    log.debug("Getting sections of the codes.")
    log.debug("------------------------------")

    log.debug(f"Looking for codes in '{codes}'.")
    log.debug(f"Found {len(folders)} items.")

    for folder in folders:
        log.debug(f"Looking for codes in '{folder}'.")

        if not folder.is_dir():
            log.debug(f"'{folder}' is not a directory. Skipping.")
            continue

        sections[folder.stem]
        files = list(folder.rglob("*.cpp"))

        log.debug(f"Found {len(files)} codes in '{folder}'.")

        for code in folder.rglob("*.cpp"):
            log.debug(
                (
                    f"Found code '{code.stem}' in '{folder}'. "
                    f"Adding to '{folder.stem}' section."
                )
            )
            sections[folder.stem].append(code)

    return dict(sections)


def generate_notebook(template: str, sections: SectionsDict) -> str:
    """Generate the notebook using the template and the sections.

    Arguments
    ---------
    template: :class:`str`
        The template of the notebook.
    sections: :class:`dict`
        The sections of the codes.

    Returns
    -------
    :class:`str`
        The notebook content.
    """
    content = ""

    log.debug("----------------------------")
    log.debug("Generating notebook content.")
    log.debug("----------------------------")

    log.debug(f"Found {len(sections)} sections.")

    for section, codes in sections.items():
        log.debug(f"Generating content for '{section}' section.")
        log.debug(f"Found {len(codes)} codes for '{section}' section.")

        section_name = normalize_name(section)
        content += rf"\\subsection{{{section_name}}}\n"

        for code in codes:
            log.debug(f"Generating content for '{code.stem}' code.")

            code_name = normalize_name(code.stem)
            content += rf"\\includes{{{code_name}}}{{{code}}}\n"

        content += "\n"

    log.debug("Inserting content into the template.")
    return sub(r"%%\sINSERT\sCONTENT\sHERE\s%%", content, template)


def remove_aux() -> None:
    """Remove the auxiliary files generated by LaTeX.

    This function removes the following files:
    - tmp_notebook.aux
    - tmp_notebook.log
    - tmp_notebook.toc
    - tmp_notebook.tex
    - texput.log
    """
    items = [
        "tmp_notebook.aux",
        "tmp_notebook.log",
        "tmp_notebook.toc",
        "tmp_notebook.tex",
        "texput.log",
    ]

    for item in items:
        Path(item).unlink(missing_ok=True)


def main() -> None:
    template = get_template()
    log.info("Template loaded successfully.")

    sections = get_sections()
    log.info("Sections loaded successfully.")

    notebook = generate_notebook(template, sections)
    log.info("Notebook generated successfully.")

    with Path("tmp_notebook.tex").open("w", encoding="utf-8") as f:
        f.write(notebook)
    log.info("Temporary LaTeX file generated successfully.")

    cmd = [
        "pdflatex",
        "-interaction=nonstopmode",
        "-halt-on-error",
        "tmp_notebook.tex",
    ]

    log.info("Compiling LaTeX file to PDF.")
    log.debug("Running the following command: '%s'", " ".join(cmd))

    with Path(devnull).open("w") as f:
        try:
            # We need to run twice to generate the table of contents.
            # It's weird but it's a LaTeX thing, we can't do anything
            # about it.
            log.debug("First run to generate the table of contents.")
            subprocess.check_call(cmd, stdout=f)

            log.debug("Second run to generate the final PDF file.")
            subprocess.check_call(cmd, stdout=f)
        except Exception as exc:
            log.exception("Error while compiling LaTeX file.")
            return

    remove_aux()
    log.info("Removed auxiliary files.")

    pdf = Path("tmp_notebook.pdf")
    pdf.rename("notebook.pdf")
    log.info("Renamed the PDF file to 'notebook.pdf'.")

    log.info("Notebook generated successfully.")


if __name__ == "__main__":
    main()
