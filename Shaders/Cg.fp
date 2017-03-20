#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;


struct LuzDirecional {
  vec3 Direcao;
  vec3 Ambiente;
  vec3 Difuso;
  vec3 Especular;   
};

struct PontoLuz {
  vec3 Posicao;
  float Constante;
  float Linear;
  float Quadratico;
  vec3 Ambiente;
  vec3 Difuso;
  vec3 Especular;
};

#define NR_POINT_LIGHTS 10

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 parametros;
uniform LuzDirecional luzesDirecionais;
uniform PontoLuz luzPoints[NR_POINT_LIGHTS];

uniform bool AtivaEspecular;
uniform bool AtivaDifuso;
uniform bool AtivaAmbiente;

// parametros(0=>intensidadeLuzAmbiente, 1=>intensidadeLuzespecular)

vec3 calculaDirecaoLuz(LuzDirecional luz, vec3 Normal, vec3 direcaoVisualizacao)
{

  float IntensidadeLuz = parametros[0];
  float parametroReflexaoEspecular = parametros[1];

  vec3 direcaoLuz = normalize(-luz.Direcao);
  
  // shading diffuse
  float diff = max(dot(Normal, direcaoLuz), 0.0);

  vec3 reflectDir = reflect(-direcaoLuz, Normal);

  float spec = pow(max(dot(direcaoVisualizacao, reflectDir), 0.0), parametroReflexaoEspecular);

  vec3 Ambient = luz.Ambiente * IntensidadeLuz;
  vec3 Diffuse = luz.Difuso * Diff;
  vec3 Especular = luz.Especular * spec;

  vec3 Resultado;  
  if (AtivaAmbiente)  Resultado += Ambiente;
  if (AtivaDifuso)    Resultado += Difuso;    
  if (AtivaEspecular) Resultado += Especular;

  return Resultado;
}

// Calcula o ponto de luz. 
vec3 calculaPontoLuz(PontoLuz light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    float IntensidadeLuz = parametros[0];
    float parametroReflexaoEspecular = parametros[1];

    vec3 lightDir = normalize(light.Posicao - fragPos);    
    // Difuso 
    float diff = max(dot(normal, lightDir), 0.0);    
    // Especualr 
    vec3 reflectDir = reflect(-lightDir, normal);
    int parametroReflexaoEspecular = 32;
    float spec = pow(max(dot(direcaoVisualizacao, reflectDir), 0.0), parametroReflexaoEspecular);    
    // Atenuacao
    float distance    = length(light.Posicao - fragPos);
    float attenuation = 1.0f / (light.Constante + light.Linear * distance + 
  			     light.Quadratico * (distance * distance));    

    // Combine os resultados
    vec3 Ambiente = light.Ambiente * IntensidadeLuz;
    vec3 Difuso   = light.Difuso * diff;
    vec3 Especular = light.Especular * spec;

    Ambiente *= attenuation;
    Difuso *= attenuation;
    Especular *= attenuation;

    vec3 Resultado;  
    if (AtivaAmbiente)  Resultado += Ambiente;
    if (AtivaDifuso)    Resultado += Difuso;    
    if (AtivaEspecular) Resultado += Especular;

    return Resultado;
} 

void main(){
  
  // Cor resultante.
  //color = ourColor;

  vec3 norm = normalize(Normal);
  vec3 ViewDirection = normalize(viewPos - FragPos);

  vec3 result = calculaDirecaoLuz(luzesDirecionais, norm, ViewDirection);

  // pontos de luz.
  for (int i = 0; i < NR_POINT_LIGHTS; i++)
  {
    result += calculaPontoLuz(luzPoints[i], norm, FragPos, viewDir);
  }

  color = vec4(result, 1.0f);
}