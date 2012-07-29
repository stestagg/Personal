#!/bin/bash

INDIR=out/stage1
OUTDIR=out/scaled

TARGET=$1



./scale $INDIR/$TARGET.png $OUTDIR/$TARGET.down2.png 0.9409
./scale $INDIR/$TARGET.png $OUTDIR/$TARGET.down.png 0.97

./scale $INDIR/$TARGET.png $OUTDIR/$TARGET.up.png 1.0309278350515464200
./scale $INDIR/$TARGET.png $OUTDIR/$TARGET.up2.png 1.0628122010840686
