#pragma once
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

namespace aproch
{
    namespace fui
    {
        class FUICanvas;

        /**
         * @brief FUI»ù±¾ÔªËØ
         */
        class APROCH_API FUIElement : public QOpenGLWidget, protected QOpenGLExtraFunctions
        {
        public:
            explicit FUIElement(FUICanvas* canvas = nullptr);
            ~FUIElement();

        protected:
            virtual void initializeGL() override;
            virtual void resizeGL(int w, int h) override;
            virtual void paintGL() override;

        private:
            Q_DISABLE_COPY(FUIElement)
        };
    }
}
