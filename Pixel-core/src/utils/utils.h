#pragma once

#include "fileutils.h"
#include "imageload.h"
#include "timer.h"

template <typename T = int>
int findElement(const std::vector<T>& vector, const T& element) {

	auto iterator = std::find(vector.begin(), vector.end(), element);
	if (iterator != vector.end()) {
		return distance(vector.begin(), iterator);
	}

	return -1;
}