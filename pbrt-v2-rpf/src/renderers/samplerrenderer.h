
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef PBRT_RENDERERS_SAMPLERRENDERER_H
#define PBRT_RENDERERS_SAMPLERRENDERER_H

// renderers/samplerrenderer.h*
#include "pbrt.h"
#include "renderer.h"
#include "parallel.h"

// SamplerRenderer Declarations
class SamplerRenderer : public Renderer {
public:
    // SamplerRenderer Public Methods
    SamplerRenderer(Sampler *s, Camera *c, SurfaceIntegrator *si,
                    VolumeIntegrator *vi, bool visIds);
    ~SamplerRenderer();
    void Render(const Scene *scene);
    Spectrum Li(const Scene *scene, const RayDifferential &ray,
        const Sample *sample, RNG &rng, MemoryArena &arena,
        Intersection *isect = NULL, Spectrum *T = NULL, bool isSpecular = false, float rWeight = 1.0f, float gWeight = 1.0f, float bWeight = 1.0f) const;
    Spectrum Transmittance(const Scene *scene, const RayDifferential &ray,
        const Sample *sample, RNG &rng, MemoryArena &arena) const;
private:
    // SamplerRenderer Private Data
    bool visualizeObjectIds;
    Sampler *sampler;
    Camera *camera;
    SurfaceIntegrator *surfaceIntegrator;
    VolumeIntegrator *volumeIntegrator;
};



// SamplerRendererTask Declarations
class SamplerRendererTask : public Task {
public:
    // SamplerRendererTask Public Methods
    SamplerRendererTask(const Scene *sc, Renderer *ren, Camera *c,
                        ProgressReporter &pr, Sampler *ms, Sample *sam, 
                        bool visIds, int tn, int tc)
      : reporter(pr)
    {
        scene = sc; renderer = ren; camera = c; mainSampler = ms;
        origSample = sam; visualizeObjectIds = visIds; taskNum = tn; taskCount = tc;
    }
    void Run();
private:
    // SamplerRendererTask Private Data
    const Scene *scene;
    const Renderer *renderer;
    Camera *camera;
    Sampler *mainSampler;
    ProgressReporter &reporter;
    Sample *origSample;
    bool visualizeObjectIds;
    int taskNum, taskCount;
};



#endif // PBRT_RENDERERS_SAMPLERRENDERER_H
