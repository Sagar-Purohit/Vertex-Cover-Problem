import matplotlib.pyplot as graph_plot

fig, graph_figure = graph_plot.subplots()
vertice = [3,5,7,9,10,11,13,15,16]
meanArray = [0.05058,0.22464,0.93212,5.9746,19.7255,59.91784,138.2908,3576.752,19424.340]
stdArray = [0.0297439,0.09720306,0.5846233333,4.491786667,33.25792,58.96404,137.432,4050.846667,14117.94667]


graph_plot.title("CNF-SAT")
graph_plot.xlabel("Number of Vertices", fontsize = "15")
graph_plot.ylabel("Running Time(ms)", fontsize = "15")

graph_plot.plot(vertice,meanArray, label = "CNF-SAT")
graph_figure.errorbar(vertice,meanArray, stdArray, fmt = 'o',marker='.',capsize = 3, label = "Standard Deviation for CNF-SAT")

graph_plot.legend(loc='upper left')
graph_plot.show()