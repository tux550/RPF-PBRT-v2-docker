
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


// integrators/path.cpp*
#include "stdafx.h"
#include "integrators/path.h"
#include "scene.h"
#include "intersection.h"
#include "paramset.h"
#include "../core/material.h"
#include "../materials/glass.h"
#include "../materials/substrate.h"
#include "../SampleWriter/SampleWriter.h" // MOD

// PathIntegrator Method Definitions
void PathIntegrator::RequestSamples(Sampler *sampler, Sample *sample,
                                    const Scene *scene) {
    for (int i = 0; i < SAMPLE_DEPTH; ++i) {
        lightSampleOffsets[i] = LightSampleOffsets(1, sample);
        lightNumOffset[i] = sample->Add1D(1);
        bsdfSampleOffsets[i] = BSDFSampleOffsets(1, sample);
        pathSampleOffsets[i] = BSDFSampleOffsets(1, sample);
    }
}

Spectrum PathIntegrator::Li(const Scene *scene, const Renderer *renderer,
        const RayDifferential &r, const Intersection &isect,
        const Sample *sample, RNG &rng, MemoryArena &arena, bool isSpecular, float rWeight, float gWeight, float bWeight) const {
    // Declare common path integration variables
    Spectrum pathThroughput = 1., L = 0.;
    RayDifferential ray(r);
    bool specularBounce = false;
    Intersection localIsect;
    const Intersection *isectp = &isect;
	BSDF *firstBSDF = NULL;
	bool isGlass = false;
	bool haveSavedTex2 = false; 
	bool shouldSendGlassRays = true;
    for (int bounces = 0; ; ++bounces) {

        // Possibly add emitted light at path vertex
        if (bounces == 0 || specularBounce)
            L += pathThroughput * isectp->Le(-ray.d);

        // Sample illumination from lights to find path contribution
		GeometricPrimitive* gp = (GeometricPrimitive*)(isectp->primitive);
		Material* m = (Material*) gp->material.GetPtr();
		float reflectProb = m->getReflectanceProb();
	
		GlassMaterial* gm = dynamic_cast<GlassMaterial*>(m);
		if(bounces == 0 && gm) {
			isGlass = true;
		}

		bool saveTexture2 = false;
		if((bounces != 0 && !gm && !haveSavedTex2) || (bounces == 0 && isSpecular && !haveSavedTex2)) {
			saveTexture2 = true;
			haveSavedTex2 = true;
		}

        BSDF *bsdf = isectp->GetBSDF(ray, arena, bounces, isSpecular, saveTexture2, rWeight, gWeight, bWeight);//1, 1, 1);
		if(bounces == 0) {
			firstBSDF = bsdf;
		}

		if(r.depth == -1) {
			break;
		}

        const Point &p = bsdf->dgShading.p;
        const Normal &n = bsdf->dgShading.nn;
        Vector wo = -ray.d;
		Spectrum LTemp;
        if (bounces < SAMPLE_DEPTH) {
			int bounceNum = bounces;
			if(isSpecular) {
				bounceNum = -1;
			}
            LTemp = pathThroughput *
                 UniformSampleOneLight(scene, renderer, arena, p, n, wo,
                     isectp->rayEpsilon, ray.time, bsdf, sample, rng,
                     lightNumOffset[bounces], &lightSampleOffsets[bounces],
                     &bsdfSampleOffsets[bounces], bounceNum, isSpecular);
		} else
            LTemp = pathThroughput *
                 UniformSampleOneLight(scene, renderer, arena, p, n, wo,
                     isectp->rayEpsilon, ray.time, bsdf, sample, rng);

		L += LTemp;
        // Sample BSDF to get new path direction

        // Get _outgoingBSDFSample_ for sampling new path direction
        BSDFSample outgoingBSDFSample;

		#if SAVE_SAMPLES

			if(bounces == 1) {

				Point tempP = p;//isectp->dg.p;
				Normal tempN = n;//isectp->dg.nn;

				if(Dot(ray.d, tempN) > 0) {
					tempN.x *= -1.0f;
					tempN.y *= -1.0f;
					tempN.z *= -1.0f;
				}

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, tempP.x, WORLD_2_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, tempP.y, WORLD_2_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, tempP.z, WORLD_2_Z_OFFSET);

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, tempN.x, NORM_2_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, tempN.y, NORM_2_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, tempN.z, NORM_2_Z_OFFSET);

			}

		#endif
		

        if (bounces < SAMPLE_DEPTH)
            outgoingBSDFSample = BSDFSample(sample, pathSampleOffsets[bounces],
                                            0);
        else
            outgoingBSDFSample = BSDFSample(rng);

        Vector wi;
        float pdf;
        BxDFType flags;
		Vector* diffRay = new Vector(0, 0, 0);
		
        Spectrum f = bsdf->Sample_f(wo, &wi, outgoingBSDFSample, &pdf,
                                    BSDF_ALL, &flags, diffRay, reflectProb);

        specularBounce = (flags & BSDF_SPECULAR) != 0;
        pathThroughput *= f * AbsDot(wi, n) / pdf;
        ray = RayDifferential(p, wi, ray, isectp->rayEpsilon);
		if (f.IsBlack() || pdf == 0.) {
			delete diffRay;
            break;
		}

		delete diffRay;
        // Possibly terminate the path
        if (bounces > 3) {
            float continueProbability = min(.5f, pathThroughput.y());
            if (rng.RandomFloat() > continueProbability) {
                break;
			}
            pathThroughput /= continueProbability;
        }
        if (bounces == maxDepth) {
			break;
		}

		shouldSendGlassRays = true;
        // Find next vertex of path
        if (!scene->Intersect(ray, &localIsect)) {
            if (specularBounce)
                for (uint32_t i = 0; i < scene->lights.size(); ++i) {
                   L += pathThroughput * scene->lights[i]->Le(ray);
				   shouldSendGlassRays = false;
				}
            break;
        }
        pathThroughput *= renderer->Transmittance(scene, ray, NULL, rng, arena);
        isectp = &localIsect;

		
		if (bounces == 0 && specularBounce)
			break;

    }

	// Handle the special case with glass. Extra samples for reflection and transmittance are needed for low spp's

	if(shouldSendGlassRays && specularBounce && r.depth + 1 < maxDepth && r.depth != -1) { 

        Vector wi;
        // Trace rays for specular reflection and refraction
        L +=  SpecularReflect(r, firstBSDF, rng, isect, renderer, scene, sample,
                             arena, rWeight, gWeight, bWeight);

        L += SpecularTransmit(r, firstBSDF, rng, isect, renderer, scene, sample,
                             arena, rWeight, gWeight, bWeight);
    } 


    return L;
}

PathIntegrator *CreatePathSurfaceIntegrator(const ParamSet &params) {
    int maxDepth = params.FindOneInt("maxdepth", 5);
    return new PathIntegrator(maxDepth);
}


