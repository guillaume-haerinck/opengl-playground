#pragma once

#include "examples/i-example.h"
#include "core/context.h"
#include "systems/i-system.h"

namespace basicExample {
    class ModelLoading : public IExample {
    public:
        ModelLoading(Context& context);
        ~ModelLoading();
        
        virtual void update() override;
        virtual void imGuiUpdate() override;

    private:
        Context& m_ctx;
        std::vector<std::shared_ptr<ISystem>> m_systems;
    };
}

