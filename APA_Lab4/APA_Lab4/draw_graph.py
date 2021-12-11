#!/bin/python
import networkx as nx
import matplotlib.pyplot as plt
import csv

color = ['#282828', #0 black
         '#7c6f64', #1 grey
         '#cc241d', #2 red
         '#fb4934', #3 red light
         '#98971a', #4 green-bolotnii
         '#b8bb26', #5 green-bolotnii light
         '#d79921', #6 orange
         '#fabd2f', #7 orange light
         '#458588', #8 green-blue
         '#83a598', #9 green-grey
         '#b16286', #10 pink
         '#d3869b', #11 pink light
         '#689d6a', #12 green
         '#8ec07c', #13 emerald
         '#a89984', #14 brown
         '#fbf1c7', #15 yellow
         '#886CE4', #16 purple
         '#2279D4'] #17 blue

Edge_Color = color[13] 
Node_Color = color[16] 
Font_Color = color[0] 
Node_Size = 550
Edge_Weight = 1.5
Node_Font = 12
Edge_Font = 10
Edge_Pos = 0.4

layout = [nx.spring_layout,         #0
          nx.random_layout,         #1
          nx.shell_layout,          #2
          nx.spectral_layout,       #3
          nx.circular_layout,       #4
          nx.spiral_layout,         #5
          nx.kamada_kawai_layout,   #6
          nx.planar_layout]         #7

Idex_Layout = 0

def plot_graph(graph, result):


    plt.figure('Graful initial')
    g = nx.Graph()
    for i in range(len(graph)):
        for j in range(len(graph[i])):
            if graph[i][j] != 0:
                g.add_edge(i, j, weight=graph[i][j])

    edge_labels = dict([((n1, n2), d['weight']) for n1, n2, d in g.edges(data=True)])

    pos = layout[Idex_Layout](g)

    nx.draw(g, pos, with_labels=True, node_size=Node_Size, font_size=Node_Font, font_color=Font_Color,
            node_color=Node_Color,edge_color=Edge_Color, width=Edge_Weight)

    nx.draw_networkx_edge_labels(g, pos, edge_labels=edge_labels,
                            font_color=Font_Color, font_size=Edge_Font, label_pos=Edge_Pos)

    

    plt.figure('Arbore de acoperire de cost minim')

    g = nx.Graph()
    for i in range(len(result)):
        for j in range(len(result[i])):
            if result[i][j] != 0:
                g.add_edge(i, j, weight=result[i][j])

    edge_labels = dict([((n1, n2), d['weight']) for n1, n2, d in g.edges(data=True)])

    pos = layout[Idex_Layout](g)

    nx.draw(g, pos, with_labels=True, node_size=Node_Size, font_size=Node_Font, font_color=Font_Color,
            node_color=Node_Color,edge_color=Edge_Color, width=Edge_Weight)

    nx.draw_networkx_edge_labels(g, pos, edge_labels=edge_labels,
                            font_color=Font_Color, font_size=Edge_Font, label_pos=Edge_Pos)
    
    plt.show()


def import_graph(filename):  
    file = []
    with open(filename,'r') as csvfile:
        plots = csv.reader(csvfile, delimiter = ',')
        for row in plots:
            irow = []
            for e in row:
                irow.append(int(e))
            file.append(irow)
    return file

graph = import_graph('graph.csv')
result = import_graph('result.csv')

plot_graph(graph, result)
