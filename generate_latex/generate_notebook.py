import logging
from pathlib import Path

from click import command, option
from rich.logging import RichHandler

log = logging.getLogger(__name__)


def get_template(name: str) -> str:
    """Get the template file as a string.

    Arguments
    ---------
    name: :class:`str`
        The name of the template file.

    Returns
    -------
    :class:`str`
        The content of the template file

    Raises
    ------
    :class:`FileNotFoundError`
        The template file does not exist.
    """
    log.debug(f"Getting the template file: [red]{name}[/]")
    f = Path(name)

    if not f.exists():
        raise FileNotFoundError(f"The template file {name} does not exist.")

    return f.read_text(encoding="utf-8")


@command()
@option("--path", "-p", default="codes", help="Path to the codes.")
@option(
    "--template",
    "-t",
    default="templates/notebook.tex",
    help="The path to the template file.",
)
@option("--output", "-o", default="notebook.pdf", help="The output file.")
@option("--debug", "-d", is_flag=True, help="Enable debug mode.")
def main(path: str, template: str, output: str, debug: bool) -> None:
    """Entrypoint of the script."""
    handler = RichHandler(
        rich_tracebacks=True, omit_repeated_times=False, markup=True
    )

    log.setLevel(logging.DEBUG if debug else logging.INFO)
    log.addHandler(handler)

    log.info("Starting the script.")

    try:
        template = get_template(template)
    except FileNotFoundError:
        log.exception(f"The template file [red]{template}[/] does not exist.")


if __name__ == "__main__":
    main()
