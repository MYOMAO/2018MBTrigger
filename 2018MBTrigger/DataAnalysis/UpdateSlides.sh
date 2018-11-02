cp plots*/Day1Task/*png AutoGenSlides/Day1Task/Plots
cd AutoGenSlides/Day1Task
pdflatex -file-line-error -halt-on-error Day1Task.tex
cd ..
cd ..

cp plotspp/EnergySum/*png AutoGenSlides/EnergySum/Plots/pp
cp plotsXeXe/EnergySum/*png AutoGenSlides/EnergySum/Plots/XeXe
cp plotsPbPb/EnergySum/*png AutoGenSlides/EnergySum/Plots/PbPb

cd AutoGenSlides/EnergySum
pdflatex -file-line-error -halt-on-error EnergySum.tex
cd ..
cd ..

