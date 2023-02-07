#!/bin/bash

for file in $(find src/ -name '*.cpp')
do
    echo $file
    xgettext --keyword=_ --language=C++ --add-comments -j --from-code="utf-8" --sort-output -o locale/Dusk2D.pot $file
done
