#!/bin/sh
if [ -z "$1" ]; then
    echo "Usage: ./run-stealth.sh document.tex"
    exit 1
fi
sed -E '/^%\[CRYPT\]/ { s/^%\[CRYPT\]//; y/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM/; }' "$1" > .render_tmp.tex
pdflatex .render_tmp.tex
mv .render_tmp.pdf "$(basename "$1" .tex).pdf"
rm -f .render_tmp.*
echo "Done! Secure PDF generated."
