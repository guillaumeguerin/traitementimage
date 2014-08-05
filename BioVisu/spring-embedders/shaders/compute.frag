#version 130

uniform sampler2D Tcur;
uniform sampler2D Tnext;
uniform sampler2D Tnodes;
uniform sampler2D TedgesList;

uniform int verticesTexturesSize;
uniform int neighboursTexturesSize;
uniform float Eltime;

in vec4 position_obj;
in vec4 position_view;
in vec2 texcoord;

out vec4 out_color;

float rand(vec2 n)
{
  return 0.5 + 0.5 *
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main()
{

   vec4 curCoords = texture2D(Tcur, texcoord.st);
   vec4 nextCoords = texture2D(Tnext, texcoord.st);
   vec4 nodesCoords = texture2D(Tnodes, texcoord.st);
   vec4 neighbours = texture2D(TedgesList, texcoord.st);

   // calculate forces and motions

  // out_color = texture(Tcur, texcoord);
   out_color = vec4(nextCoords.r+rand(vec2(position_obj.xy))+Eltime, curCoords.r+rand(vec2(position_obj.xy)),curCoords.r+rand(vec2(position_obj.xy)), 1);
}
