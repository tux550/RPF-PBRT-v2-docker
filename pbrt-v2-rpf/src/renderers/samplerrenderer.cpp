
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


// renderers/samplerrenderer.cpp*
#include "stdafx.h"
#include "renderers/samplerrenderer.h"
#include "scene.h"
#include "film.h"
#include "volume.h"
#include "sampler.h"
#include "integrator.h"
#include "progressreporter.h"
#include "camera.h"
#include "intersection.h"
#include "../SampleWriter/SampleWriter.h"	// MOD
#include "samplers/random.h"				// MOD

char sceneName[BUFFER_SIZE];				// MOD
int pbrtSamplesPerPixel;					// MOD
extern bool isRandomSampler;				// MOD
int* numSamplesCompleted;					// MOD

static uint32_t pbrt_hash(char *key, uint32_t len)
{
    uint32_t hash = 0, i;
    for (hash=0, i=0; i<len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
} 

// SamplerRendererTask Definitions
void SamplerRendererTask::Run() {
    PBRT_STARTED_RENDERTASK(taskNum);
    // Get sub-_Sampler_ for _SamplerRendererTask_
    Sampler *sampler = mainSampler->GetSubSampler(taskNum, taskCount);
    if (!sampler)
    {
        reporter.Update();
        PBRT_FINISHED_RENDERTASK(taskNum);
        return;
    }

    // Declare local variables used for rendering loop
    MemoryArena arena;
	
	srand(time(NULL) + taskNum);
    RNG rng(taskNum);

    // Allocate space for samples and intersections
    int maxSamples = sampler->MaximumSampleCount();
    Sample *samples = origSample->Duplicate(maxSamples);
    RayDifferential *rays = new RayDifferential[maxSamples];
    Spectrum *Ls = new Spectrum[maxSamples];
    Spectrum *Ts = new Spectrum[maxSamples];
    Intersection *isects = new Intersection[maxSamples];

    // Get samples from _Sampler_ and update image
    int sampleCount;

    while ((sampleCount = sampler->GetMoreSamples(samples, rng)) > 0) {

			
		if(numSamplesCompleted[taskNum] > 1000) {
			reporter.Update(numSamplesCompleted[taskNum]);
			numSamplesCompleted[taskNum] = 0;
		}
		numSamplesCompleted[taskNum] += sampleCount;
		//reporter.Update(sampleCount);

        // Generate camera rays and compute radiance along rays
        for (int i = 0; i < sampleCount; ++i) {

			//********************** MOD ************************//

			#if SAVE_SAMPLES
				
				if(isRandomSampler) {
					RandomSampler* rSampler = (RandomSampler*) sampler;
					samples[i].sampleNum = rSampler->getSamplePos() - 1;
				} else {
					samples[i].sampleNum = i;
				}
				arena.setX((size_t) samples[i].imageX);
				arena.setY((size_t) samples[i].imageY);
				arena.setSampleNum((size_t) samples[i].sampleNum);
				
			#endif 

			//***************************************************//

            // Find camera ray for _sample[i]_
            PBRT_STARTED_GENERATING_CAMERA_RAY(&samples[i]);
			
			#if SAVE_SAMPLES

				SampleWriter::setPosition((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, samples[i].imageX, X_COORD_OFFSET); // MOD
				SampleWriter::setPosition((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, samples[i].imageY, Y_COORD_OFFSET); // MOD
			
			#endif
			
            float rayWeight = camera->GenerateRayDifferential(samples[i], &rays[i]);
            rays[i].ScaleDifferentials(1.f / 16);
            PBRT_FINISHED_GENERATING_CAMERA_RAY(&samples[i], &rays[i], rayWeight);

            // Evaluate radiance along camera ray
            PBRT_STARTED_CAMERA_RAY_INTEGRATION(&rays[i], &samples[i]);
            if (visualizeObjectIds) {
                if (rayWeight > 0.f && scene->Intersect(rays[i], &isects[i])) {
                    // random shading based on shape id...
                    uint32_t ids[2] = { isects[i].shapeId, isects[i].primitiveId };
                    uint32_t h = pbrt_hash((char *)ids, sizeof(ids));
                    float rgb[3] = { float(h & 0xff), float((h >> 8) & 0xff),
                                     float((h >> 16) & 0xff) };
                    Ls[i] = Spectrum::FromRGB(rgb);
                    Ls[i] /= 255.f;
                }
                else
                    Ls[i] = 0.f;
            }
            else {
            if (rayWeight > 0.f) {
                Ls[i] = rayWeight * renderer->Li(scene, rays[i], &samples[i], rng,
                                                 arena, &isects[i], &Ts[i]);

			} else {

                Ls[i] = 0.f;
                Ts[i] = 1.f;

            }

            // Issue warning if unexpected radiance value returned
            if (Ls[i].HasNaNs()) {
                Error("Not-a-number radiance value returned "
                      "for image sample.  Setting to black.");
                Ls[i] = Spectrum(0.f);
            }
            else if (Ls[i].y() < -1e-5) {
                Error("Negative luminance value, %f, returned"
                      "for image sample.  Setting to black.", Ls[i].y());
                Ls[i] = Spectrum(0.f);
            }
            else if (isinf(Ls[i].y())) {
                Error("Infinite luminance value returned"
                      "for image sample.  Setting to black.");
                Ls[i] = Spectrum(0.f);
            }
            }

			//********************** MOD ************************//

			#if SAVE_SAMPLES

				SampleWriter::setRandomParameter((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, samples[i].lensU, U_COORD_OFFSET);
				SampleWriter::setRandomParameter((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, samples[i].lensV, V_COORD_OFFSET);
				SampleWriter::setRandomParameter((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, samples[i].time, TIME_OFFSET); 

				float rgb[NUM_OF_COLORS];
				Ls[i].ToRGB(rgb);
			
				SampleWriter::setColor((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, rgb[0], COLOR_1_OFFSET); 
				SampleWriter::setColor((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, rgb[1], COLOR_2_OFFSET); 
				SampleWriter::setColor((size_t) samples[i].imageX, (size_t) samples[i].imageY, samples[i].sampleNum, rgb[2], COLOR_3_OFFSET); 

			#endif
				
			//***************************************************//

            PBRT_FINISHED_CAMERA_RAY_INTEGRATION(&rays[i], &samples[i], &Ls[i]);
        }

        // Report sample results to _Sampler_, add contributions to image
        if (sampler->ReportResults(samples, rays, Ls, isects, sampleCount))
        {
            for (int i = 0; i < sampleCount; ++i)
            {
                PBRT_STARTED_ADDING_IMAGE_SAMPLE(&samples[i], &rays[i], &Ls[i], &Ts[i]);
                camera->film->AddSample(samples[i], Ls[i]);
                PBRT_FINISHED_ADDING_IMAGE_SAMPLE();
            }
        }

        // Free _MemoryArena_ memory from computing image sample values
        arena.FreeAll();
    }

    // Clean up after _SamplerRendererTask_ is done with its image region
    camera->film->UpdateDisplay(sampler->xPixelStart,
        sampler->yPixelStart, sampler->xPixelEnd+1, sampler->yPixelEnd+1);
    delete sampler;
    delete[] samples;
    delete[] rays;
    delete[] Ls;
    delete[] Ts;
    delete[] isects;
    reporter.Update();
    PBRT_FINISHED_RENDERTASK(taskNum);
}



// SamplerRenderer Method Definitions
SamplerRenderer::SamplerRenderer(Sampler *s, Camera *c,
                                 SurfaceIntegrator *si, VolumeIntegrator *vi,
                                 bool visIds) {
    sampler = s;
    camera = c;
    surfaceIntegrator = si;
    volumeIntegrator = vi;
    visualizeObjectIds = visIds;
}


SamplerRenderer::~SamplerRenderer() {
    delete sampler;
    delete camera;
    delete surfaceIntegrator;
    delete volumeIntegrator;
}


void SamplerRenderer::Render(const Scene *scene) {
    PBRT_FINISHED_PARSING();
    // Allow integrators to do preprocessing for the scene
    PBRT_STARTED_PREPROCESSING();
    surfaceIntegrator->Preprocess(scene, camera, this);
    volumeIntegrator->Preprocess(scene, camera, this);
    PBRT_FINISHED_PREPROCESSING();
    PBRT_STARTED_RENDERING();
    // Allocate and initialize _sample_
    Sample *sample = new Sample(sampler, surfaceIntegrator,
                                volumeIntegrator, scene);

    // Create and launch _SamplerRendererTask_s for rendering image

    // Compute number of _SamplerRendererTask_s to create for rendering
    int nPixels = camera->film->xResolution * camera->film->yResolution;
	int nTasks;
	nTasks = max(32 * NumSystemCores(), nPixels / (16*16)); 
	nTasks = RoundUpPow2(nTasks); 
	#if SAVE_SAMPLES

		SampleWriter::initialize((size_t) camera->film->xResolution, (size_t) camera->film->yResolution, (size_t) sampler->samplesPerPixel); // MOD
	
	#endif

	int numOfSamples = camera->film->xResolution * camera->film->yResolution * sampler->samplesPerPixel;

    ProgressReporter reporter(numOfSamples, "Rendering");
    vector<Task *> renderTasks;
	numSamplesCompleted = new int[nTasks];
	memset(numSamplesCompleted, 0, nTasks * sizeof(int));
    for (int i = 0; i < nTasks; ++i)
        renderTasks.push_back(new SamplerRendererTask(scene, this, camera,
                                                      reporter, sampler, sample, 
                                                      visualizeObjectIds, 
                                                      nTasks-1-i, nTasks));
    EnqueueTasks(renderTasks);
    WaitForAllTasks();
    for (uint32_t i = 0; i < renderTasks.size(); ++i)
        delete renderTasks[i];
	delete[] numSamplesCompleted;
    reporter.Done();
    PBRT_FINISHED_RENDERING();

    // Clean up after rendering and store final image
    delete sample;

	//********************** MOD ************************//

	#if SAVE_SAMPLES

		
		SampleWriter::ProcessData(sceneName); // MOD

	#endif

	//***************************************************//

}


Spectrum SamplerRenderer::Li(const Scene *scene,
        const RayDifferential &ray, const Sample *sample, RNG &rng,
        MemoryArena &arena, Intersection *isect, Spectrum *T, bool isSpecular, float rWeight, float gWeight, float bWeight) const {
    Assert(ray.time == sample->time);
    Assert(!ray.HasNaNs());
    // Allocate local variables for _isect_ and _T_ if needed
    Spectrum localT;
    if (!T) T = &localT;
    Intersection localIsect;
    if (!isect) isect = &localIsect;
    Spectrum Li = 0.f;
    if (scene->Intersect(ray, isect)) {
	
        Li = surfaceIntegrator->Li(scene, this, ray, *isect, sample,
                                   rng, arena, isSpecular, rWeight, gWeight, bWeight);

		#if SAVE_SAMPLES

			if(!isSpecular) {

				BSDF* bsdf = isect->GetBSDF(ray, arena, -1);
				Point p = bsdf->dgShading.p;
				Normal n = bsdf->dgShading.nn;

				if(Dot(ray.d, n) > 0) {
					n.x *= -1.0f;
					n.y *= -1.0f;
					n.z *= -1.0f;
				} 

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, p.x, WORLD_1_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, p.y, WORLD_1_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, p.z, WORLD_1_Z_OFFSET);

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, n.x, NORM_1_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, n.y, NORM_1_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, n.z, NORM_1_Z_OFFSET);

			}

		#endif 

	} else {

		#if SAVE_SAMPLES
			if(!isSpecular) {
				LightSampleOffsets* lightOffset = surfaceIntegrator->getLightOffset();
				BSDFSampleOffsets* bsdfOffset = surfaceIntegrator->getBSDFOffset();
				int lightNumOffset = surfaceIntegrator->getLightNumOffset();
				BSDFSampleOffsets* pathOffset = surfaceIntegrator->getPathOffset();
				if(lightOffset != NULL) { 
					SampleWriter::setRandomParameter((size_t) arena.getX(), (size_t) arena.getY(), arena.getSampleNum(), sample->twoD[lightOffset->posOffset][0], LIGHT_1_OFFSET);
					SampleWriter::setRandomParameter((size_t) arena.getX(), (size_t) arena.getY(), arena.getSampleNum(), sample->twoD[lightOffset->posOffset][1], LIGHT_2_OFFSET);
				}

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, WORLD_1_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, WORLD_1_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, WORLD_1_Z_OFFSET);

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, NORM_1_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, NORM_1_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, NORM_1_Z_OFFSET);

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, TEXTURE_1_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, TEXTURE_1_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, TEXTURE_1_Z_OFFSET);

				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, TEXTURE_2_X_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, TEXTURE_2_Y_OFFSET);
				SampleWriter::setFeature((size_t) sample->imageX, (size_t) sample->imageY, sample->sampleNum, 0, TEXTURE_2_Z_OFFSET);
		
			}
		#endif

        // Handle ray that doesn't intersect any geometry
        for (uint32_t i = 0; i < scene->lights.size(); ++i)
           Li += scene->lights[i]->Le(ray);
    }
    Spectrum Lvi = volumeIntegrator->Li(scene, this, ray, sample, rng,
                                        T, arena);
    return *T * Li + Lvi;
}

Spectrum SamplerRenderer::Transmittance(const Scene *scene,
        const RayDifferential &ray, const Sample *sample, RNG &rng,
        MemoryArena &arena) const {
    return volumeIntegrator->Transmittance(scene, this, ray, sample,
                                           rng, arena);
}


