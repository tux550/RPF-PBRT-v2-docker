
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


// materials/plastic.cpp*
#include "stdafx.h"
#include "materials/plastic.h"
#include "spectrum.h"
#include "reflection.h"
#include "paramset.h"
#include "texture.h"
#include "../SampleWriter/SampleWriter.h" // MOD

// PlasticMaterial Method Definitions
BSDF *PlasticMaterial::GetBSDF(const DifferentialGeometry &dgGeom,
                               const DifferentialGeometry &dgShading,
                               MemoryArena &arena, int bounceNum, bool isSpecularBounce, bool saveTexture2, float rWeight, float gWeight, float bWeight) const {
    // Allocate _BSDF_, possibly doing bump mapping with _bumpMap_
    DifferentialGeometry dgs;
    if (bumpMap)
        Bump(bumpMap, dgGeom, dgShading, &dgs);
    else
        dgs = dgShading;
    BSDF *bsdf = BSDF_ALLOC(arena, BSDF)(dgs, dgGeom.nn);
    Spectrum kd = Kd->Evaluate(dgs).Clamp();
    if (!kd.IsBlack()) {
        BxDF *diff = BSDF_ALLOC(arena, Lambertian)(kd);
        bsdf->Add(diff);
    }
    Spectrum ks = Ks->Evaluate(dgs).Clamp();
    if (!ks.IsBlack()) {
        Fresnel *fresnel = BSDF_ALLOC(arena, FresnelDielectric)(1.5f, 1.f);
        float rough = roughness->Evaluate(dgs);
        BxDF *spec = BSDF_ALLOC(arena, Microfacet)
                       (ks, fresnel, BSDF_ALLOC(arena, Blinn)(1.f / rough));
        bsdf->Add(spec);
    }
	
	//********************** MOD ************************//

	#if SAVE_SAMPLES

		if(!isSpecularBounce) {
			if(bounceNum == 0) {

				float rgb[NUM_OF_COLORS];
				kd.ToRGB(rgb);

				float currentR = SampleWriter::getFeature(arena.getX(), arena.getY(), arena.getSampleNum(), TEXTURE_1_X_OFFSET);
				float currentG = SampleWriter::getFeature(arena.getX(), arena.getY(), arena.getSampleNum(), TEXTURE_1_Y_OFFSET);
				float currentB = SampleWriter::getFeature(arena.getX(), arena.getY(), arena.getSampleNum(), TEXTURE_1_Z_OFFSET);

				if(currentR == 0 && currentG == 0 && currentB == 0) {
					SampleWriter::setFeature(arena.getX(), arena.getY(), arena.getSampleNum(), rgb[0], TEXTURE_1_X_OFFSET);
					SampleWriter::setFeature(arena.getX(), arena.getY(), arena.getSampleNum(), rgb[1], TEXTURE_1_Y_OFFSET);
					SampleWriter::setFeature(arena.getX(), arena.getY(), arena.getSampleNum(), rgb[2], TEXTURE_1_Z_OFFSET);
				}
		
			} 
		}
		
		if(saveTexture2) {

			float rgb[NUM_OF_COLORS];
			kd.ToRGB(rgb);
				
			float currentR = SampleWriter::getFeature(arena.getX(), arena.getY(), arena.getSampleNum(), TEXTURE_2_X_OFFSET);
			float currentG = SampleWriter::getFeature(arena.getX(), arena.getY(), arena.getSampleNum(), TEXTURE_2_Y_OFFSET);
			float currentB = SampleWriter::getFeature(arena.getX(), arena.getY(), arena.getSampleNum(), TEXTURE_2_Z_OFFSET);

			SampleWriter::setFeature(arena.getX(), arena.getY(), arena.getSampleNum(), currentR + rWeight * rgb[0], TEXTURE_2_X_OFFSET);
			SampleWriter::setFeature(arena.getX(), arena.getY(), arena.getSampleNum(), currentG + gWeight * rgb[1], TEXTURE_2_Y_OFFSET);
			SampleWriter::setFeature(arena.getX(), arena.getY(), arena.getSampleNum(), currentB + bWeight * rgb[2], TEXTURE_2_Z_OFFSET);
		
		}

	#endif

	//***************************************************//

	return bsdf;
}


PlasticMaterial *CreatePlasticMaterial(const Transform &xform,
        const TextureParams &mp) {
    Reference<Texture<Spectrum> > Kd = mp.GetSpectrumTexture("Kd", Spectrum(0.25f));
    Reference<Texture<Spectrum> > Ks = mp.GetSpectrumTexture("Ks", Spectrum(0.25f));
    Reference<Texture<float> > roughness = mp.GetFloatTexture("roughness", .1f);
    Reference<Texture<float> > bumpMap = mp.GetFloatTextureOrNull("bumpmap");
    return new PlasticMaterial(Kd, Ks, roughness, bumpMap);
}


