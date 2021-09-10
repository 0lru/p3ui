# Copyright (c) 2021 Martin Rudoff
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os
import argparse
from pathlib import Path

parser = argparse.ArgumentParser(description='add a header/license from a template to some files')

parser.add_argument('template', type=str, help='template file containing the license')

parser.add_argument('-e', '--extensions', nargs='*',
                    help='list of file extensions (example: .hpp .cpp)')

parser.add_argument('-c', '--check-only', action='store_true',
                    help='do not modify any file, just check')

parser.add_argument('-d', '--directories', nargs='*',
                    help='list of directories to parse')

parser.add_argument('-r', '--remove', action='store_true',
                    help='remove template from specified files')

args = parser.parse_args()

template = None

with open(args.template) as f:
    template = f.read()


def update_license(path):
    with open(path.as_posix()) as f:
        content = f.read()
        is_present = content[0:len(template)] == template
        if args.remove:
            if not is_present:
                return
            print(f'[rem] {path}')
            if args.check_only:
                return
            with open(path, "w") as out:
                out.write(content[len(template):])
        else:
            if is_present:
                return
            print(f'[add] {path}')
            if args.check_only:
                return
            with open(path, "w") as out:
                out.write(template + content)


for directory in args.directories:
    for current_dir, directories, filenames in os.walk(directory):
        for filename in filenames:
            path = Path(current_dir).joinpath(filename)
            if path.suffix in args.extensions:
                update_license(path)
