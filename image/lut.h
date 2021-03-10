/***
 * Copyright 2019 The Katla Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef LUT_H
#define LUT_H

#include <atomic> 

namespace katla {

class Lut
{
public:
    Lut (void);
    Lut (int depth);
    Lut (const Lut& lut);

    virtual ~Lut (void);

    Lut& operator= (const Lut& lut);

    int depth (void) const {return m_depth;}

    int* red (void) const {return m_red;}
    int* green (void) const {return m_green;}
    int* blue (void) const {return m_blue;}
private:
    int m_depth;

    int* m_red {nullptr};
    int* m_green {nullptr};
    int* m_blue {nullptr};

    std::atomic<int>* m_ref {nullptr};
};

}

#endif // LUT_H
