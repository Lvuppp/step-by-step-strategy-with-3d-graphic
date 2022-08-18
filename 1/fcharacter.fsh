struct materialProperty {

    vec3 diffuseColor,
         ambienceColor,
        specularColor;

    float shinnes;
};

uniform sampler2D u_diffuseMap, u_normalMap;

uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
uniform highp materialProperty u_materialPropery;

uniform bool u_isUsingDiffuseMap, u_isUsingNormalMap;

varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;


void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);

    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);

    vec4 diffMatColor = /*vec4(1.0, 0.0, 0.0, 1.0);*/ texture2D(u_diffuseMap, v_texcoord);

    vec3 usingNormal = v_normal;

    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz); //вектор направления взгляда,
            //направлен из 0 0 0 (заданная точка) в точку, которую сейчас обрабатывает фр. шейдер


    vec3 LightVect = normalize(v_position.xyz - u_lightPosition.xyz); //вектор света. направлен из точки,
                                            //где находится источник освещения к вектору позиции

    if (u_isUsingNormalMap) {

        usingNormal = normalize(texture2D(u_normalMap, v_texcoord).rgb * 2.0 - 1.0); //* 2.0 - 1.0 : переход к интервалу от -1.0 до 1.0
        //eyeVect = normalize(v_tbnMatrix * eyeVect); //здесь след для тени

        //LightVect = normalize(v_tbnMatrix * eyeVect); //здесь крч тень ТЕНЬ но черная
    }


    vec3 reflectLight = normalize(reflect(LightVect, usingNormal));//вектор, направленный из точки, которую мы рассматриваем,
                                                                //в направлении, куда отразится этот свет

    float len = length(v_position.xyz - eyePosition.xyz);//расстояние от наблюдателя до рассматриваемой точки

    float specularFactor = u_materialPropery.shinnes; //то, насколько большим будет пятно блика (60.0)

    float ambientFactor = 0.1; //насколько сильно светится сам по себе материал

    if (!u_isUsingDiffuseMap)
        diffMatColor = vec4(u_materialPropery.diffuseColor, 1.0); //1.0 - полностью непрозрачен

    //чтобы убрать затухание света при отдалении объекта, нужно убрать деление
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -LightVect)) /*/ (1.0 + 0.025 * len * len)*/;

    resultColor += diffColor;

    resultColor *= vec4(u_materialPropery.ambienceColor, 1.0);

    //следующие две строки - уходим от диффузного освещения:
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;
    resultColor *= vec4(u_materialPropery.ambienceColor, 1.0);

    //добавляем блики (и здесь тоже убрать деление, чтобы избавиться от затухания света):
    vec4 specularColor = /* цвет блика: */ vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower *
            pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor) /*/ (1.0 + 0.025 * len * len)*/; //делим, чтобы с удалением слабее освещалось и влияние источника света было меньше
    resultColor += specularColor;
    resultColor *= vec4(u_materialPropery.specularColor, 1.0);

    gl_FragColor = resultColor; //texture2D(u_texture, v_texcoord);
}
