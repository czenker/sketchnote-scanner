#!/usr/bin/env python3
#coding=utf-8
import sys
import os
import xml.etree.ElementTree as ET

def usage():
  print(sys.argv[0] + " [directory] [outfile]", file=sys.stderr)

if len(sys.argv) < 3:
  usage()
  sys.exit(1)

if not os.path.exists(sys.argv[1] + "/black.svg"):
  print("Source directory does not include a readable black.svg.", file=sys.stderr)
  usage()
  sys.exit(1)


ET.register_namespace("","http://www.w3.org/2000/svg")
ns = { 'svg': "http://www.w3.org/2000/svg" }


def add_layer(name, root):
  path = sys.argv[1] + "/" + name + ".svg"
  if os.path.isfile(path):
    print("adding " + name)
    tree = ET.parse(path)
    layer = tree.getroot()
    
    g = layer.find("svg:g", ns)
    g.set("id", name)
    root.append(g)



tree = ET.parse(sys.argv[1] + "/black.svg")
root = tree.getroot()

root.remove(root.find("svg:metadata", ns))  
black = root.find("svg:g", ns)
black.set("id", "black")
  
rect = ET.Element("svg:rect", ns)
rect.set("id", "white")
rect.set("width", "100%")
rect.set("height", "100%")
rect.set("fill", "white")
root.insert(root.getchildren().index(black), rect)

add_layer("sanguine", root)
add_layer("red", root)
add_layer("cobaltGreen", root)
add_layer("phthaloBlue", root)
add_layer("indanthreneBlue", root)
add_layer("mayGreen", root)

tree.write(sys.argv[2])

sys.exit(0)