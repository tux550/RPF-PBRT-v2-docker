
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

#ifndef PBRT_MATERIALS_MIXMAT_H
#define PBRT_MATERIALS_MIXMAT_H

// materials/mixmat.h*
#include "pbrt.h"
#include "material.h"

// MixMaterial Declarations
class MixMaterial : public Material {
public:
    // MixMaterial Public Methods
    MixMaterial(Reference<Material> mat1, Reference<Material> mat2,
                Reference<Texture<Spectrum> > sc)
        : m1(mat1), m2(mat2), scale(sc) {
    }
    BSDF *GetBSDF(const DifferentialGeometry &dgGeom,
                  const DifferentialGeometry &dgShading,
                  MemoryArena &arena, int bounceNum, bool isSpecularBounce = false, bool saveTexture2 = false, float rWeight = 1.0f, float gWeight = 1.0f, float bWeight = 1.0f) const;

	float getReflectanceProb() {
		return -1.0f;
	}

private:
    // MixMaterial Private Data
    Reference<Material> m1, m2;
    Reference<Texture<Spectrum> > scale;
};


MixMaterial *CreateMixMaterial(const Transform &xform,
    const TextureParams &mp, const Reference<Material> &m1,
    const Reference<Material> &m2);

#endif // PBRT_MATERIALS_MIXMAT_H
