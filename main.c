// { begin copyright } 
// Copyright Ryan Marcus 2016
// 
// This file is part of js-constellation.
// 
// js-constellation is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// js-constellation is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with js-constellation.  If not, see <http://www.gnu.org/licenses/>.
// 
// { end copyright } 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define RAND_RANGE(a, b) (b - ((float)rand() / (float)RAND_MAX)*(b-a))
#define SIGN(f) (f < 0.0 ? -1 : 1)
#define LINE_BUFFER_MAX 100

typedef struct {
    float x;
    float y;
    float xv;
    float yv;
} Star;


typedef struct {
    float xBound;
    float yBound;
    int numStars;
    Star* stars;
} Sky; // it's full of stars.

int starCompare(const void* star1, const void* star2) {
    Star* s1 = (Star*) star1;
    Star* s2 = (Star*) star2;

    return SIGN(s1->x - s2->x);
};

void isort(void* base, size_t nel, size_t width,
           int (*compar)(const void*, const void*)) {

    char tmp[width];
    if (nel < 2)
        return;

    for (size_t i = 1; i < nel; i++) {
        size_t j = i;
        while (j > 0 && compar(base + (j-1)*width, base + j*width) > 0) {
            memcpy(tmp, base + (j - 1)*width, width);
            memcpy(base + (j - 1)*width, base + j*width, width);
            memcpy(base + j*width, tmp, width);
            j--;
        }
    }
}

int intCompare(const void* i1, const void* i2) {
    int i = *((int*) i1);
    int j = *((int*) i2);

    printf("compared %d %d \n", i, j);
    
    return i - j;
}

Sky* createSky(int numStars, unsigned int randSeed, float maxVelocity, float xBound, float yBound) {
    Sky* toR = (Sky*) malloc(sizeof(Sky));

    if (toR == NULL)
        return NULL;

    toR->xBound = xBound;
    toR->yBound = yBound;
    
    toR->stars = (Star*) malloc(sizeof(Star) * numStars);
    if (toR->stars == NULL) {
        free(toR);
        return NULL;
    }

    srand(randSeed);

    toR->numStars = numStars;
    for (Star* s = toR->stars; s < toR->stars + numStars; s++) {
        s->x = RAND_RANGE(0, xBound);
        s->y = RAND_RANGE(0, yBound);
        s->xv = RAND_RANGE(-1, 1) * maxVelocity;
        s->yv = RAND_RANGE(-1, 1) * maxVelocity;

        printf("Star velocity is %f, %f, pos: %f %f\n", s->xv, s->yv, s->x, s->y);
    }

    // sort the star array so that the stars are ordered
    // by their x coord
    qsort(toR->stars, numStars, sizeof(Star), starCompare);

    return toR;
}

Sky* progress(Sky* sky) {
    for (Star* s = sky->stars; s < sky->stars + sky->numStars; s++) {

        s->x += s->xv;
        s->y += s->yv;

        s->xv = (s->x < 0 || s->x > sky->xBound ? -(s->xv) : s->xv);
        s->yv = (s->y < 0 || s->y > sky->yBound ? -(s->yv) : s->yv);
    }

    // keep everything sorted using an insertion sort pass
    // since the ordering of the stars along the x-axis is not
    // likely to have changed very much.
    isort(sky->stars, sky->numStars, sizeof(Star), starCompare);

    return sky;
}


void drawLines(Sky* sky, float maxDistance,
               void (*drawLineBuffer) (Star** stars, int numStars)) {
    Star* lineBuffer[LINE_BUFFER_MAX];
    int currBufferSize = 0;

    

    // do spherical colli detection used sweep & scan method along the x-axis
    for (Star* i = sky->stars; i < sky->stars + sky->numStars; i++) {
        for (Star* j = i+1; j < sky->stars + sky->numStars; j++) {
            // if the x distance is too great, short circuit, check no more
            if (j->x - i->x > maxDistance) {
                break;
            }

            // otherwise, we need to check the total distance
            float dist = sqrtf(powf(i->x - j->x, 2) + powf(i->y - j->y, 2));
            if (dist < maxDistance) {
                // add it to the buffer
                if (currBufferSize >= LINE_BUFFER_MAX) {
                    drawLineBuffer(lineBuffer, currBufferSize);
                    currBufferSize = 0;
                }

                lineBuffer[currBufferSize] = i;
                lineBuffer[currBufferSize+1] = j;
                currBufferSize += 2;
            }
            
        }
    }

    drawLineBuffer(lineBuffer, currBufferSize);
}

float readStarX(Star* star) {
    return star->x;
}

float readStarY(Star* star) {
    return star->y;
}

float getStarX(Sky* sky, int idx) {
    return sky->stars[idx].x;
}

float getStarY(Sky* sky, int idx) {
    return sky->stars[idx].y;
}

Star* getStarAt(Star** stars, int idx) {
    return stars[idx];
}
