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


#include "lut.h"

#include <cstring>

namespace katla {

Lut::Lut (void) :
    m_red (nullptr),
    m_green (nullptr),
    m_blue (nullptr),
    m_ref (nullptr)
{

}

Lut::Lut (int depth) :
    m_depth (depth)
{
    int size = 1 << depth;

    m_red = new int [size];
    m_green = new int [size];
    m_blue = new int [size];

    memset (m_red, 0, sizeof (int) * size);
    memset (m_green, 0, sizeof (int) * size);
    memset (m_blue, 0, sizeof (int) * size);

    m_ref = new std::atomic<int>();
    m_ref->operator++();
}

Lut::Lut (const Lut& lut)
{
    m_depth = lut.m_depth;

    m_red = lut.m_red;
    m_green = lut.m_green;
    m_blue = lut.m_blue;

    m_ref = lut.m_ref;
    m_ref->operator++();
}

Lut::~Lut (void)
{
    if (!m_ref)
        return;

    if (!m_ref->operator--())
    {
        if (m_red)
            delete[] m_red;
        m_red = nullptr;

        if (m_green)
            delete[] m_green;
        m_green = nullptr;

        if (m_blue)
            delete[] m_blue;
        m_blue = nullptr;

        delete m_ref;
        m_ref = nullptr;
    }
}

Lut& Lut::operator= (const Lut& lut)
{
    if (&lut != this && m_ref)
    {
        if (!m_ref->operator--())
        {
            if (m_red)
                delete[] m_red;
            m_red = nullptr;

            if (m_green)
                delete[] m_green;
            m_green = nullptr;

            if (m_blue)
                delete[] m_blue;
            m_blue = nullptr;

            delete m_ref;
            m_ref = nullptr;
        }
    }

    if (lut.m_ref)
    {
        m_depth = lut.m_depth;

        m_red = lut.m_red;
        m_green = lut.m_green;
        m_blue = lut.m_blue;

        m_ref = lut.m_ref;
        m_ref->operator++();
    }

    return *this;
}

}
