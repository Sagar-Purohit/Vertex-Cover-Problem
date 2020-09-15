import matplotlib.pyplot as graph_plot

fig, graph_figure = graph_plot.subplots()

vertice = [3,5,7,9,10,11,13,15,16]
approxRatioCNF = [1,1,1,1,1,1,1,1,1]
approxRatioApproxVc1 = [1,1,1,1,1,1.03667,1.03333,1.05714,1]
approxRatioApproxVc2 = [2,1.63333,1.5,1.52,1.54333,1.51667,1.56667,1.65952,1.53571]

std_cnf = [0,0,0,0,0,0,0,0,0]
std_appx1 = [0,0,0,0,0,0.0776984,0.0702728,0.073771,0]
std_appx2 = [0,0.399073,0.304290333,0.225092667,0.2079,0.230539,0.164054,0.170976,0.192695333]

graph_plot.title("Approximation Ratio Analysis")
graph_plot.xlabel("Number of Vertices", fontsize = "15")
graph_plot.ylabel("Approximation Ratio", fontsize = "15")

graph_plot.plot(vertice,approxRatioCNF, label = "CNF-SAT")
graph_plot.plot(vertice,approxRatioApproxVc1, label = "APPROX-VC-1")
graph_plot.plot(vertice,approxRatioApproxVc2, label = "APPROX-VC-2")

graph_figure.errorbar(vertice,approxRatioCNF, std_cnf, fmt = 'o',marker='.',capsize = 3, label = "Standard Deviation for CNF-SAT")
graph_figure.errorbar(vertice,approxRatioApproxVc1, std_appx1, fmt = 'o',marker='.',capsize = 3, label = "Standard Deviation for Approx-Vc-1")
graph_figure.errorbar(vertice,approxRatioApproxVc2, std_appx2, fmt = 'o',marker='.',capsize = 3, label = "Standard Deviation for Approx-Vc-2")

graph_plot.legend(loc='upper right',prop={'size': 4}, fontsize = 'large')
graph_plot.show()