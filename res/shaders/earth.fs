#version 460

layout(location = 0) out vec4 fs_color;

in vec3 te_position;
in vec2 te_texcoord;
in mat3 te_tangentSpace;
in vec4 te_fragPosLightSpace;

uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform sampler2D lightsTex;
uniform sampler2DShadow shadowTex;

uniform vec3 lightDir;
uniform vec3 camPos;


float ShadowCalculation(in vec3 normal)
{
    vec3 projCoords = te_fragPosLightSpace.xyz / te_fragPosLightSpace.w;
    projCoords = projCoords * .5f + .5f;

    // outside of shadowmap fully unlit
    if (projCoords.z > 1.f)
        return 1.f;

    //float closestDepth = texture(shadowTex, projCoords.xy).r;
    float bias = max(.05f * (1.f - dot(normal, lightDir)), .005f); 
    
    float shadow = 0.f;
    vec2 texelSize = 1.f / textureSize(shadowTex, 0);
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(shadowTex, vec3(projCoords.xy + vec2(x, y) * texelSize, projCoords.z + 0.0001f ));
            shadow += (projCoords.z - bias > pcfDepth) ? 1.f : 0.f;
        }    
    }
    shadow /= 9.f;

    return shadow;
}

void main()
{
    vec3 normal = normalize(te_tangentSpace * (texture(normTex, te_texcoord).rgb * 2.f - 1.f));

    vec3 sunColor = vec3(.97f, .80f, .65f);

    vec3 ambient = sunColor * .1f;

    vec3 viewDir = normalize(camPos - te_position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float lightNormTess = dot(normal, lightDir);
    float lightNormBase = dot(te_tangentSpace[2], lightDir);

    float spec = pow(max(dot(viewDir, reflectDir), 0.f), 100.f);
    vec3 specular = vec3(texture(specTex, te_texcoord).r) * spec * sunColor;

    float shadow = ShadowCalculation(normal);

    float diff = max(lightNormTess, 0.f);
    vec3 diffuse = diff * texture(diffuseTex, te_texcoord).rgb;

    vec3 lights = texture(lightsTex, te_texcoord).rgb;

    vec3 day = (1.f - shadow) * (diffuse + specular);
    vec3 night = -min((lightNormBase - 1.f) * .5f, 0.f) * lights;

    fs_color = vec4(ambient + day + night, 1.f);
    //fs_color = vec4(normal, 1.f);
}
