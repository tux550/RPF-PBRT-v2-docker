#include "SSE_Wrapper.h"
#include <stdexcept> // for std::runtime_error

// MYEDIT
float getM128Sum(__m128 v) {
	__m128 temp1 = _mm_movehl_ps(v, v); // {x2, x3, -, -}
	temp1 = _mm_add_ps(v, temp1); // {x0+x2, x1+x3, -, -}
	__m128 temp2 = _mm_shuffle_ps(temp1, temp1, 1); // {x1+x3, -, -, -}
	temp1 = _mm_add_ss(temp1, temp2); // {x0+x2+x1+x3, -, -, -}
	return _mm_cvtss_f32(temp1);
}
float extractIthRegister(__m128 v, int i) {
	__m128 temp_shuffle;
	switch (i) {
		case 0:
			temp_shuffle = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)); // {x0, x0, x0, x0}
			break;
		case 1:
			temp_shuffle = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)); // {x1, x1, x1, x1}
			break;
		case 2:
			temp_shuffle = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)); // {x2, x2, x2, x2}
			break;
		case 3:
			temp_shuffle = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)); // {x3, x3, x3, x3}
			break;
		default:
			throw std::invalid_argument("Invalid index in extractIthRegister");
			break;
	}
	return _mm_cvtss_f32(temp_shuffle); // Extract the first element


	//__m128 temp_shuffle = _mm_shuffle_ps(v, v, _MM_SHUFFLE(i, i, i, i)); // {xi, xi, xi, xi}
	//return _mm_cvtss_f32(temp_shuffle); // Extract the first element
}
void setIthRegister(__m128 *v, int i, float value) {
	// REVIEW
	__m128 temp;
	switch (i) {
		case 0:
			// Use set
			*v = _mm_move_ss(*v, _mm_set_ss(value)); // {value, x1, x2, x3}
			break;
		case 1:
			// Swap 0-1 ,set and swap back
			temp = _mm_shuffle_ps(*v, *v, _MM_SHUFFLE(1, 0, 2, 3)); // {x1, x0, x2, x3}
			temp = _mm_move_ss(temp, _mm_set_ss(value)); // {value, x0, x2, x3}
			*v = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 0, 2, 3)); // {value, x1, x2, x3}
			break;
		case 2:
			// Swap 0-2 ,set and swap back
			temp = _mm_shuffle_ps(*v, *v, _MM_SHUFFLE(2, 1, 0, 3)); // {x2, x1, x0, x3}
			temp = _mm_move_ss(temp, _mm_set_ss(value)); // {value, x1, x0, x3}
			*v = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(2, 1, 0, 3)); // {value, x1, x2, x3}
			break;
		case 3:
			// Swap 0-3 ,set and swap back
			temp = _mm_shuffle_ps(*v, *v, _MM_SHUFFLE(3, 1, 2, 0)); // {x3, x1, x2, x0}
			temp = _mm_move_ss(temp, _mm_set_ss(value)); // {value, x1, x2, x0}
			*v = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(3, 1, 2, 0)); // {value, x1, x2, x3}
			break;
		default:
			throw std::invalid_argument("Invalid index in setIthRegister");

	}

}
// ----

float calculateMeanSSE(float* src, size_t length) {

	// Arg checking
	assert(length >= 4);

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;
	
	// Group the data into groups of four
    v4sf* data = (v4sf*) src;

	// Initialize the sum to zero
	v4sf cumulativeSum = _mm_set_ps1(0.0f);

	// Add up all of the elements
    for(size_t i = 0; i < loopSize; i++ ) {

		cumulativeSum = _mm_add_ps(*data, cumulativeSum);            

		data++; // Move on to the next four

    }
	
	// Sum up the four entries to get the total sum so far
	// MYEDIT
	// float sum = cumulativeSum.m128_f32[0] + cumulativeSum.m128_f32[1] + cumulativeSum.m128_f32[2] + cumulativeSum.m128_f32[3];

	//__m128 temp1 = _mm_movehl_ps(cumulativeSum, cumulativeSum); // {x2, x3, -, -}
	//temp1 = _mm_add_ps(cumulativeSum, temp1); // {x0+x2, x1+x3, -, -}
	//__m128 temp2 = _mm_shuffle_ps(temp1, temp1, 1); // {x1+x3, -, -, -}
	//temp1 = _mm_add_ss(temp1, temp2); // {x0+x2+x1+x3, -, -, -}
	//float sum = _mm_cvtss_f32(temp1);

	float sum = getM128Sum(cumulativeSum);


	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		//sum += (*data).m128_f32[i];
		//__m128 temp_shuffle = _mm_shuffle_ps(*data, *data, _MM_SHUFFLE(i, i, i, i)); // {xi, xi, xi, xi}
		//float temp_value = _mm_cvtss_f32(temp_shuffle); // Extract the first element
		//sum += temp_value;
		sum += extractIthRegister(*data, i);
	}

	// Divide by the length to calculate the mean
	float mean = sum / length;

	// Return the mean of the vector
	return mean;

}

float calculateStdSSE(float* src, float mean, size_t length) {

	// Arg checking
	assert(length >= 4);

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;
	
	// Group the data into groups of four
    v4sf* data = (v4sf*) src;

	// Set up 4 copies of the mean 
	v4sf mu = _mm_set_ps1(mean);

	// Temporary variable to hold the intermediate results
	// when calculating the standard deviation
	v4sf temp;

	// Cumulative sum of the squared differences
	v4sf cumulativeSum = _mm_set_ps1(0.0f);

	// Add up all of the elements
    for(size_t i = 0; i < loopSize; i++ ) {
         
		// Sum of squared differences
		temp = _mm_sub_ps(*data, mu);
        temp = _mm_mul_ps(temp, temp);
		cumulativeSum = _mm_add_ps(cumulativeSum, temp);

		data++; // Move on to the next four

    }
	
	// Add up the total so far

	// MYEDIT
	//float squaredSum = cumulativeSum.m128_f32[0] + cumulativeSum.m128_f32[1] + cumulativeSum.m128_f32[2] + cumulativeSum.m128_f32[3];
	float squaredSum = getM128Sum(cumulativeSum);

	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		//squaredSum += pow(((*data).m128_f32[i] - mean), 2);
		squaredSum += pow(extractIthRegister(*data, i) - mean, 2);
	}

	// Calculate the variance
	float variance = squaredSum / (length - 1); 

	// Return the square root of the variance (standard deviation)
	return sqrtf(variance);

}

void normalizeSSE(float* src, float mean, float std, float* result, size_t length) {

	// Arg checking
	assert(length >= 4);

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;
	float stdInv;
	if(fabs(std) > EPSILON) {
		stdInv = 1.0f / std;
	} else {
		stdInv = 1.0f / EPSILON;
	}
	
	// Group the data into groups of four
    v4sf* data = (v4sf*) src;
	v4sf* dest = (v4sf*) result;

	// Set up 4 copies of the mean and standard deviation
	v4sf mu = _mm_set_ps1(mean);
	v4sf sigmaInv = _mm_set_ps1(stdInv);

	// Add up all of the elements
    for(size_t i = 0; i < loopSize; i++ ) {
         
		*dest = _mm_sub_ps(*data, mu);
		*dest = _mm_mul_ps(*dest, sigmaInv);

		data++; // Move on ot the next four
		dest++;

    }

	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		//(*dest).m128_f32[i] = ((*data).m128_f32[i] - mean) * stdInv;
		float temp_val = extractIthRegister(*data, i);
		float temp_res = (temp_val - mean) * stdInv;
		setIthRegister(dest, i, temp_res);

	}

}

void subtractSquareScaleSSE(float* data1, float* data2, float* result, float alpha, size_t length) {

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;

	// Group the data into groups of four
    v4sf* d1 = (v4sf*) data1;
	v4sf* d2 = (v4sf*) data2;
	v4sf* dest = (v4sf*) result;
	v4sf scalar = _mm_set_ps1(alpha);
	v4sf temp;

	// Filter the colors of samples in pixel using bilateral filter
	for(size_t i = 0; i < loopSize; i++) {
 
		temp = _mm_sub_ps(*d1, *d2);
		temp = _mm_mul_ps(temp, temp);
		*dest = _mm_mul_ps(temp, scalar);
		
		dest++; // Move on to the next four
		d1++;
		d2++;

	}
		
	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		//(*dest).m128_f32[i] = alpha * pow(((*d1).m128_f32[i] - (*d2).m128_f32[i]), 2);
		float temp_val1 = extractIthRegister(*d1, i);
		float temp_val2 = extractIthRegister(*d2, i);
		float temp_res = alpha * pow(temp_val1 - temp_val2, 2);
		setIthRegister(dest, i, temp_res);
	} 

}

void sumSSE(float* data1, float* data2, float* result, size_t length) {

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;

	// Group the data into groups of four
    v4sf* d1 = (v4sf*) data1;
	v4sf* d2 = (v4sf*) data2;
	v4sf* dest = (v4sf*) result;

	// Filter the colors of samples in pixel using bilateral filter
	for(size_t i = 0; i < loopSize; i++) {
 
		*dest = _mm_add_ps(*d1, *d2);

		dest++; // Move on to the next four
		d1++;
		d2++;
		
	}
		
	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		//(*dest).m128_f32[i] = (*d1).m128_f32[i] + (*d2).m128_f32[i];
		float temp_val1 = extractIthRegister(*d1, i);
		float temp_val2 = extractIthRegister(*d2, i);
		float temp_res = temp_val1 + temp_val2;
		setIthRegister(dest, i, temp_res);
	}

}

void multByValSSE(float* data, float* result, float alpha, size_t length) {

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;

	// Group the data into groups of four
    v4sf* d = (v4sf*) data;
	v4sf* dest = (v4sf*) result;
	v4sf scalar = _mm_set_ps1(alpha);
	
	// Filter the colors of samples in pixel using bilateral filter
	for(size_t i = 0; i < loopSize; i++) {
 
		*dest = _mm_mul_ps(*d, scalar);
		
		dest++; // Move on to the next four
		d++;

	}
		
	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		//(*dest).m128_f32[i] = alpha * (*d).m128_f32[i];
		float temp_val = extractIthRegister(*d, i);
		float temp_res = alpha * temp_val;
		setIthRegister(dest, i, temp_res);

	}

}

void multSSE(float* data1, float* data2, float* result, size_t length) {

	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;

	// Group the data into groups of four
    v4sf* d1 = (v4sf*) data1;
	v4sf* d2 = (v4sf*) data2;
	v4sf* dest = (v4sf*) result;
	
	// Filter the colors of samples in pixel using bilateral filter
	for(size_t i = 0; i < loopSize; i++) {
 
		*dest = _mm_mul_ps(*d1, *d2);
		
		dest++; // Move on to the next four
		d1++;
		d2++;

	}
		
	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {


		// MYEDIT
		//(*dest).m128_f32[i] = (*d1).m128_f32[i] * (*d2).m128_f32[i];
		float temp_val1 = extractIthRegister(*d1, i);
		float temp_val2 = extractIthRegister(*d2, i);
		float temp_res = temp_val1 * temp_val2;
		setIthRegister(dest, i, temp_res);

	}

}

float sumAllElementsSSE(float* data, size_t length) {
	
	// Calculate how many groups of four there are
	// and how many extras there are
	size_t loopSize = length / 4;
	size_t numOfExtras = length % 4;

	// Group the data into groups of four
    v4sf* d = (v4sf*) data;
	v4sf temp = _mm_set1_ps(0.0f);
	
	// Filter the colors of samples in pixel using bilateral filter
	for(size_t i = 0; i < loopSize; i++) {
 
		temp = _mm_add_ps(*d, temp); 
		
		d++; // Move on to the next four
	}
	
	// MYEDIT
	//float result = temp.m128_f32[0] + temp.m128_f32[1] + temp.m128_f32[2] + temp.m128_f32[3];
	float result = getM128Sum(temp);

	// Add in the extras
	for(size_t i = 0; i < numOfExtras; i++) {
		// MYEDIT
		// result += (*d).m128_f32[i];
		result += extractIthRegister(*d, i);
	}

	return result;

}
