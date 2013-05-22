#!/bin/bash

rm -rf $1/$2/headers 

mkdir $1/$2/headers

#cp $(find $1/src -name *.h) $1/$2/headers/
rsync -pavr                                                 \
    --include '+ */' --include '*.h' --exclude '- *'      \
    $1/src/ $1/$2/headers/
