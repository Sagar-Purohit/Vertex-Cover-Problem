import matplotlib.pyplot as graph_plot

fig, graph_figure = graph_plot.subplots()

vertice = [3,5,7,9,10,11,13,15,16]

mean_array_appx1 = [0.00866,0.02148,0.02126,0.0286,0.03618,0.03758,0.0448,0.05796,0.06668]
std_array_appx1 = [0.00545378,0.007990296,0.00741516,0.009014846667,0.0175389,0.01076071,0.00982456,0.007313786667,0.00788872]

mean_array_appx2 = [0.00726,0.01494,0.01542,0.01986,0.02276,0.02112,0.02488,0.02808,0.03136]
std_array_appx2 = [0.00168155,0.005642154,0.004904589333,0.01141847333,0.006129908,0.0039924226,0.006921248,0.004355314667,0.004711017333]

graph_plot.title("APPROX-VC-1 AND APPROX-VC-2")
graph_plot.xlabel("Number of Vertices", fontsize = "15")
graph_plot.ylabel("Running Time(ms)", fontsize = "15")

graph_plot.plot(vertice,mean_array_appx1, label = "APPROX-VC-1")
graph_figure.errorbar(vertice,mean_array_appx1, std_array_appx1, fmt = 'o',marker='.',capsize = 3, label = "Standard Deviation for Approx-Vc-1")

graph_plot.plot(vertice,mean_array_appx2, label = "APPROX-VC-2")
graph_figure.errorbar(vertice,mean_array_appx2, std_array_appx2, fmt = 'o',marker='.',capsize = 3, label = "Standard Deviation for Approx-Vc-2")

graph_plot.legend(loc='upper left')
graph_plot.show()