#!/bin/bash
for i in *tga
do
    name=${i%.tga}
    convert "$name.tga[0]"  "$name.png"
done
