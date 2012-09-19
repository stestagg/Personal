#!/bin/bash
set -e

INDIR=out/color
OUTDIR=out/scaled

TARGET=$(basename $1 .png)


./scale_rgb $INDIR/$TARGET.png $OUTDIR/$TARGET.down.png 0.97
./scale_rgb $INDIR/$TARGET.png $OUTDIR/$TARGET.up.png 1.0309278350515464200
