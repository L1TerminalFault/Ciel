#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float itemWidth;
    float itemHeight;
    vec4 surfaceColor;
    vec4 borderColor;
    float borderWidth;
    float r;
    float p;
    float aa;
};

float superellipseNorm(vec2 v, float exp) {
    vec2 absV = abs(v);
    return pow(pow(absV.x, exp) + pow(absV.y, exp), 1.0 / exp);
}

void main() {
    vec2 size = vec2(itemWidth, itemHeight);
    vec2 halfSize = size * 0.5;
    vec2 pos = abs(qt_TexCoord0 * size - halfSize);

    float cornerRadius = clamp(r, 0.0, min(halfSize.x, halfSize.y));
    vec2 cornerBox = halfSize - vec2(cornerRadius);
    vec2 d = pos - cornerBox;

    float dist;
    if (d.x > 0.0 && d.y > 0.0) {
        dist = superellipseNorm(d, p) - cornerRadius;
    } else {
        dist = max(d.x, d.y) - cornerRadius;
    }

    float delta = fwidth(dist);
    float outerAlpha = clamp(0.5 - dist / max(delta, 0.001), 0.0, 1.0);

    vec4 finalColor;
    if (borderWidth > 0.0) {
        float innerDist = dist + borderWidth;
        float borderDelta = fwidth(innerDist);
        float innerAlpha = clamp(0.5 - innerDist / max(borderDelta, 0.001), 0.0, 1.0);
        finalColor = mix(borderColor, surfaceColor, innerAlpha);
    } else {
        finalColor = surfaceColor;
    }

    // Correct premultiplied alpha: Prevents dirty dark/gray color fringing
    float combinedAlpha = outerAlpha * finalColor.a * qt_Opacity;
    fragColor = vec4(finalColor.rgb * combinedAlpha, combinedAlpha);
}
