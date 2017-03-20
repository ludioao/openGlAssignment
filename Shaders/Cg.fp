#version 130
out vec4 color;
uniform vec4 ourColor;

in vec3 FragPos;
in vec3 Normal;
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
  
  // Cor resultante.
  //color = ourColor;

  // Ambiente
  float AmbientStr = 0.1f;
  vec3 Ambiente = AmbientStr * lightColor;

  // LightDir.
  vec3 norm = normalize(Normal);
  vec3 LightDir = normalize(lightPos- FragPos);
  float diff = max(dot(norm, LightDir), 0.0);
  vec3 Diffuse = diff * lightColor;


  // Especular.
  float SpecularStr = 0.5f;
  vec3 ViewDirection = normalize(viewPos - FragPos);
  vec3 RefDirection = reflect(-LightDir, norm);
  float Spec = pow(max(dot(ViewDirection, RefDirection), 0.0), 32);
  vec3 Specular = SpecularStr * Spec * lightColor;

  vec3 result = (Ambiente + Diffuse + Specular) * ourColor;
  color = vec4(result, 1.0f);
}