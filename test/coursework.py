# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


import glob
import nbconvert
import nbformat
import os.path
import runpy


def test(file):
    print(os.path.basename(file))

    notebook = nbformat.read(file, as_version=4)
    code, metadata = nbconvert.PythonExporter().from_notebook_node(notebook)

    # Don't perform ipython magic
    adjusted_code = 'import matplotlib\nmatplotlib.use("Agg")\n'
    for line in code.split('\n'):
        if not line.startswith('get_ipython'):
            adjusted_code += line + '\n'

    # Don't open graph windows
    adjusted_code = adjusted_code.replace('plt.show', 'plt.draw')

    script_file = os.path.splitext(os.path.basename(file))[0] + '.pytest'
    print(adjusted_code, file=open(script_file, 'w'))
    runpy.run_path(script_file)


if __name__ == '__main__':
    for file in glob.glob('../coursework/*.ipynb'):
        test(file)
