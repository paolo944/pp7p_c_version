#!/bin/bash
uglifyjs public/js/scripts.js -o public/js/scripts.min.js
uglifyjs public/js/subtitles.js -o public/js/subtitles.min.js
postcss public/css/style.css --use cssnano -o public/css/style.min.css
postcss public/css/subtitles.css --use cssnano -o public/css/subtitles.min.css
gzip -k public/css/style.min.css public/js/scripts.min.js public/index.html public/icon/favicon.ico public/js/subtitles.min.js public/css/subtitles.min.css