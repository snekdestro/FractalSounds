uniform float res;
uniform vec3 col;
uniform float shift;
uniform float iter;
uniform float zoom;
uniform float dx;
uniform float dy;
uniform float brightness;
uniform vec2 c;
vec2 mult(vec2 a, vec2 b){
    return vec2(a.x *b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}


void main()
{
    
    float u = (gl_FragCoord.x-shift) / res * 4 - 2.0;
    float v = (gl_FragCoord.y) / res * 4 - 2.0;

    vec2 z  = vec2(u,-v);
    z *= zoom;
    z += vec2(dx,-dy);
    bool esc = false;
    float i = 0.0;
    for(; i  < iter; i++){
        z = mult(z,z);
        z = z + c;
        if(length(z) >= 2.0){
            esc = true;
            break;
        }
    }
    

    

    gl_FragColor = esc? vec4(col  *  vec3(i / brightness), 1.0) : vec4(0.0,0.0,0.0,1.0);

}