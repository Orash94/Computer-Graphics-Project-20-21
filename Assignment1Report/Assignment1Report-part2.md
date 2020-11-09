---
typora-root-url: pictures
---

## Mesh vertices and faces

![](/face_vertices.png)

## Scale and translate the vertices

###### Translating 

we want to know the center of the object and change its center such that it center will be at the origin of the coordinate system.

We went through a loop on all vertices and concluded the max min value for each dimension ,then we performed  (min + max)/2 , this gave us the center points between the farthest points in each dimension, after performing this for each dimension you get a point Vc,  this points represent the center , we want the center to be in V0 = (0,0,0) to do that we need to translate  the object , we create a vector (V0 - Vc) , we transform this vector to a translate transformation and apply it to the model.



###### Scaling

TODO



###### After transformation



## Renderer object

![](/MeshViewer_iS4iIydhX6.png)

## GUI

![](/MeshViewer_xPrJFAXWU0.png)



the GUI is representing in tree structure, the user has to load a model, once is it loaded the GUI selects it as the active model and two option is opened, Object transformation and World transformation, each are identical GUI wise. each transformation has three options , scaling [2,-2] , rotating [-180 ,180] , translating [ 2 ,-2].

## loading several models

![](/MeshViewer_yc7mqGrOlJ.png)

The user can select the active model he want to work on by clicking the model name in the GUI and it applies  the transformation to the model.