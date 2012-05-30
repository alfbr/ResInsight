//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2012 Ceetron AS
//    
//   This library is free software: you can redistribute it and/or modify 
//   it under the terms of the GNU General Public License as published by 
//   the Free Software Foundation, either version 3 of the License, or 
//   (at your option) any later version. 
//    
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY 
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
//   FITNESS FOR A PARTICULAR PURPOSE.   
//    
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>> 
//   for more details. 
//
//##################################################################################################

#include "cafEffectGenerator.h"
#include "cafEffectCache.h"

namespace caf {


//==================================================================================================
//
// EffectCache
//
//==================================================================================================

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
EffectCache::EffectCache()
{
    m_effectType = EffectGenerator::SHADER_BASED;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
EffectCache* EffectCache::instance()
{
    static EffectCache staticInstance;

    return &staticInstance;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
cvf::ref<cvf::Effect> EffectCache::getOrCreateEffect(const EffectGenerator* generator)
{
    CVF_ASSERT(generator);

    // Effect cache does not support mixing of effect types. Clear cache if type changes.
    if (EffectGenerator::renderingMode() != m_effectType)
    {
        clear();
        m_effectType = EffectGenerator::renderingMode();
    }

    size_t i;
    for (i = 0; i < m_effectCache.size(); i++)
    {
        if (m_effectCache[i].first->isEqual(generator))
        {
            cvf::ref<cvf::Effect> effect = m_effectCache[i].second;
            EffectGenerator* effGen = m_effectCache[i].first;
            effGen->updateEffect(effect.p());

            return effect;
        }
    }

    EffectGenerator* myCopy = generator->copy();

    cvf::ref<cvf::Effect> eff = generator->generateEffect();
    m_effectCache.push_back(std::make_pair(myCopy, eff));

    return eff;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void EffectCache::clear()
{
    size_t i;
    for (i = 0; i < m_effectCache.size(); i++)
    {
        EffectGenerator* effGenerator = m_effectCache[i].first;
        delete effGenerator;
    }

    m_effectCache.clear();
}


}