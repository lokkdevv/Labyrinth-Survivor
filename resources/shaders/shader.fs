#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;

out vec4 finalColor;

const float CURVATURE = 0.045;
const float BLOOM_STRENGTH = 0.30;
const float BLOOM_THRESHOLD = 0.70;
const float SCANLINE_STRENGTH = 0.45;
const float CHROMATIC_STRENGTH = 1.2;

vec3 getBright(vec2 uv)
{
    vec3 color = texture(texture0, uv).rgb;

    float brightness = max(color.r, max(color.g, color.b));
    float contribution = smoothstep(BLOOM_THRESHOLD, 1.0, brightness);

    return color * contribution;
}

vec3 getBloom(vec2 uv, vec2 texel)
{
    vec3 bloom = vec3(0.0);

    bloom += getBright(uv) * 4.0;

    bloom += getBright(uv + vec2(texel.x * 2.0, 0.0)) * 2.0;
    bloom += getBright(uv - vec2(texel.x * 2.0, 0.0)) * 2.0;
    bloom += getBright(uv + vec2(0.0, texel.y * 2.0)) * 2.0;
    bloom += getBright(uv - vec2(0.0, texel.y * 2.0)) * 2.0;

    bloom += getBright(uv + vec2(texel.x * 4.0, 0.0));
    bloom += getBright(uv - vec2(texel.x * 4.0, 0.0));
    bloom += getBright(uv + vec2(0.0, texel.y * 4.0));
    bloom += getBright(uv - vec2(0.0, texel.y * 4.0));

    return bloom / 16.0;
}

void main()
{
    vec2 resolution = vec2(textureSize(texture0, 0));
    vec2 texel = 1.0 / resolution;
    vec2 p = fragTexCoord * 2.0 - 1.0;

    p *= 1.0 + vec2(
        p.y * p.y,
        p.x * p.x
    ) * CURVATURE;

    vec2 uv = p * 0.5 + 0.5;

    if (uv.x < 0.0 || uv.x > 1.0 ||
        uv.y < 0.0 || uv.y > 1.0)
    {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec2 ca = p * texel * CHROMATIC_STRENGTH;

    float r = texture(texture0, uv + ca).r;
    float g = texture(texture0, uv).g;
    float b = texture(texture0, uv - ca).b;

    vec3 color = vec3(r, g, b);

    color += getBloom(uv, texel) * BLOOM_STRENGTH;

    color = color * 1.18;
    color = (color - 0.5) * 1.05 + 0.5;

    float scanline = 1.0 -
        SCANLINE_STRENGTH *
        (0.5 + 0.5 * sin(
            fragTexCoord.y * resolution.y * 3.14159
        ));

    color *= scanline;

    float phosphor = 0.90 + 0.10 * sin(
        fragTexCoord.y * resolution.y * 3.14159 * 2.0
    );

    color *= phosphor;

    float vignette = 1.0 - smoothstep(
        0.65,
        1.35,
        length(p * vec2(0.85, 0.85))
    );

    color *= vignette;

    color = clamp(color, 0.0, 1.0);

    finalColor = vec4(color, 1.0);
}





