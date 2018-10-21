cp plots*/Day1Task/*png AutoGenSlides/Plots
cd AutoGenSlides
pdflatex -file-line-error -halt-on-error Day1Task.tex
cd ..
