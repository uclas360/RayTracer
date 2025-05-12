/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** core compute
*/

#include "RaytracerCore.hpp"

void RaytracerCore::computeMoving(size_t start, size_t end) {
    for (size_t i = start; i < end; i++) {
        this->computePixel(this->compressedImage_, i,
                           this->compressedXResolution_,
                           this->compressedYResolution_);
    }
}

void RaytracerCore::computePrecision() {
    std::vector<std::uint8_t> image(this->xResolution_ * this->yResolution_ * 4,
                                    0);

    for (size_t i = 0; i < (this->xResolution_ * this->yResolution_) &&
                       !this->moving_ && !this->killThreads_;
         i++) {
        this->computePixel(image, i, this->xResolution_, this->yResolution_);
    }
    this->imageMutex_.lock();
    if (this->nbImage_ == 0) {
        this->imageMean_ = image;
    } else {
        for (size_t i = 0; (i < this->imageMean_.size()) && !this->moving_ &&
                           !this->killThreads_;
             i++) {
            this->imageMean_[i] =
                (this->imageMean_[i] * this->nbImage_ + image[i]) /
                (this->nbImage_ + 1);
        }
    }
    this->nbImage_ += 1;
    this->imageMutex_.unlock();
}

void RaytracerCore::computeImage(size_t start, size_t end) {
    do {
        if (this->moving_) {
            this->computeMoving(start, end);
        } else {
            computePrecision();
        }
    } while (!this->killThreads_ && (this->graphic_ || this->nbImage_ < 200));
}
