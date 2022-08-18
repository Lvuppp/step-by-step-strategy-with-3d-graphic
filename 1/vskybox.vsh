attribute highp vec4 a_position;
attribute highp vec2 a_texcoord;
attribute highp vec3 a_normal;

uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;

varying highp vec2 v_texcoord; //varying переменные нужны для передачи информации во фрагментный шейдер

void main(void)
{
    mat4 viewMatrix = u_viewMatrix;

    //информация о сдвигах хранится в последнем столбце матрицы

    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix * a_position;
    v_texcoord = a_texcoord;
}
