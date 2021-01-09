

## Light Types

### Ambient

<img src="./pictures/ySQoNUgzr9.png" width="400">  <img src="./pictures/tex7Lmd8qn.png" width="400">





Here we demonstrate the ambient light,  it works by multiplying the  ambient colors of light and model, because it work  the same way on point and parallel light it will show the same behavior.

### Diffuse 

#### Parallel light

 <img src="./pictures/MeshViewer_sZsHQfe460.png" width="400"> <img src="./pictures/MeshViewer_e2mNkoyxGg.png" width="400">





Diffuse color with Parallel light. We can see that the light is in the shape of an arrow indicating the direction of the light. The direction is the normal of the plane which created by the parallel light source. Applying translate transformation on the light source does not change the direction of the light, and thus no changes on the sphere. The  light should always cover half of the sphere.

#### Point light

 <img src="./pictures/MeshViewer_Pn90Td6QYD.png" width="400"> <img src="./pictures/MeshViewer_nQ2sLcmsLB.png" width="400">  

Diffuse color with point light. We can that the light is in the shape of a sphere because the light emits lights in every direction. We can see that whenever we translate the point lights source the sphere color changes,  but whenever we rotate the point light source the colors remain the same.

### Specular

#### Parallel light

<img src="./pictures/Zoom_4nBKdPWjrw.png" width="400">

Here we have the specular light with a parallel light .

#### Point light

<img src="./pictures/Zoom_P3ivz8da89.png" width="400">

Here we have the point light with a parallel light .

## Materials

<img src="./pictures/MeshViewer_K8DUKfJsFG.png" width="400">

We gave the user the options to choose material colors for the model.

## Shading types

### Flat

<img src="./pictures/chrome_XnE0uymuoY.png" width="400"> <img src="./pictures/MeshViewer_UXbjyaTIQ4.png" width="400">

We use the triangle center to color the the faces and use tringle centers to calculate colors,



### Gouraud

<img src="./pictures/chrome_exOlLbCEvz.png" width="400"> <img src="./pictures/MeshViewer_NT55zZkXbr.png" width="400">

We interpolate on the face using  three colors calculated using  three varices constructing the face and using normals per vertex per face.

### Phong

<img src="./pictures/chrome_TPTkFEEOrB.png" width="400"> <img src="./pictures/MeshViewer_896vIyibtE.png" width="400">

We interpolate nomrals of the three vertices constructing the face,and then we calculate the normal for each point on tringle and calculate color using new normal.

## Demonstration





## Post Processing Effects

### Bloom + Gaussian Blur

 <img src="./pictures/Zoom_8YYjAjRlhk.png" width="400">  <img src="./pictures/Zoom_aL5T606hhl.png" width="400">  

We allow the user to choose the threshold. Lower threshold means more ✨shinny✨ objects. source image on the left bloom effect on the right.

### Fog Effect

<img src="./pictures/Nofog.png" width="400">

Here we can see the original picture.

#### Linear

<img src="./pictures/LinearFog.png" width="400">

Users can choose the start and end of fog effect.

#### Exponential 

<img src="./pictures/expoFog.png" width="400">

Users can choose density of the fog.

#### Exponential Squared 

<img src="./pictures/expoSquaredFog.png" width="400">

Users can choose density of the fog.

### Procedural textures

<img src="./pictures/proceduralTexture.png" width="400">

