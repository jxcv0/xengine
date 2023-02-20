#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_coord_in;
layout (location = 2) in vec3 normal_in;

out vec3 pos;
out vec2 tex_coord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() { 
  tex_coord = tex_coord_in;
  normal = normalize(normal_in);
  pos = vec3(model * vec4(pos_in, 1.0));

  gl_Position = projection * view * model * vec4(pos_in, 1.0f);
}
v2fConnector distanceVertex(a2vConnector a2v, uniform float4x4 modelViewProj, uniform float3 eyeCoord,    uniform float3 lightCoord,    uniform float invBumpDepth)  {      v2fConnector v2f;      // Project position into screen space      // and pass through texture coordinate    v2f.projCoord = mul(modelViewProj, float4 (a2v.objCoord, 1));    v2f.texCoord = float3 (a2v.texCoord, 1);      // Transform the eye vector into tangent space.    // Adjust the slope in tangent space based on bump depth      float3 eyeVec = eyeCoord - a2v.objCoord;    float3 tanEyeVec;    tanEyeVec.x = dot(a2v.objTangent, eyeVec);    tanEyeVec.y = dot(a2v.objBinormal, eyeVec);    tanEyeVec.z = -invBumpDepth * dot(a2v.objNormal, eyeVec);    v2f.tanEyeVec = tanEyeVec;      // Transform the light vector into tangent space.    // We will use this later for tangent-space normal mapping      float3 lightVec = lightCoord - a2v.objCoord;    float3 tanLightVec;    tanLightVec.x = dot(a2v.objTangent, lightVec);    tanLightVec.y = dot(a2v.objBinormal, lightVec);    tanLightVec.z = dot(a2v.objNormal, lightVec);    v2f.tanLightVec = tanLightVec;      return v2f;    }

