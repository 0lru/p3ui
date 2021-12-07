import json
import platform
import shutil
import urllib.request
from pathlib import Path
from zipfile import ZipFile
from io import BytesIO

version = '0.4'
output_path = 'skia/'

output_path = Path(__file__).parent.joinpath(output_path)


class ReleaseNotFoundError(Exception):
    pass


class AssetNotFoundError(Exception):
    pass


machine = {'AMD64': 'x64', 'x86_64': 'x64', 'arm64': 'arm64'}[platform.machine()]
system = {'Darwin': 'macos', 'Linux': 'linux', 'Windows': 'windows'}[platform.system()]
build_type = 'Release'
expected_asset_name = f'skia_{system}_release_{machine}.zip'

resp = urllib.request.urlopen(urllib.request.Request(
    f'https://api.github.com/repos/0lru/p3ui_skia/releases'))
releases = json.loads(resp.fp.read())
release = next(filter(lambda r: r['name'] == version, releases), None)
if release is None:
    raise ReleaseNotFoundError(f'release {version} not found')

assets = release['assets']
asset = next(filter(lambda a: a['name'] == expected_asset_name, assets), None)
if release is None:
    raise AssetNotFoundError(f'asset {expected_asset_name} not found')

print(f'downloading "{expected_asset_name}"')
output_path = Path(output_path)
output_path.mkdir(parents=True, exist_ok=True)

with urllib.request.urlopen(asset['browser_download_url']) as response:
    bytes = response.read()
    with ZipFile(BytesIO(bytes)) as zip:
        print(f'extracting "{expected_asset_name}" to "{output_path.as_posix()}"')
        zip.extractall(output_path.as_posix())

src_dst = output_path.joinpath('out', 'Release', 'gen', 'skia.h'), output_path.joinpath('skia.h')
print('moving file "{}" to "{}"'.format(*src_dst))
shutil.move(*src_dst)
