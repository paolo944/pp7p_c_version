#!/bin/bash
uglifyjs static/scripts.js -o static/scripts.min.js
postcss static/style.css --use cssnano -o static/style.min.css
gzip -k static/style.min.css static/scripts.min.js templates/index.html static/favicon.png
