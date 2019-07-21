#ifndef FRAME_BUFFER_DEPTH_HPP
#define FRAME_BUFFER_DEPTH_HPP

class CFrameBufferDepth 
{
    public:
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HPPEIGHT = 1024;

        void Init()
        {
            // configure depth map FBO    
            glGenFramebuffers(1, &this->m_fBO);

            // create depth texture    
            glGenTextures(1, &this->m_depthMap);
            glBindTexture(GL_TEXTURE_2D, this->m_depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HPPEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, this->m_fBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->m_depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        };

        void Bind()
        {
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HPPEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, this->m_fBO);
            glClear(GL_DEPTH_BUFFER_BIT);
        };

        void Unbind()
         {
             glBindFramebuffer(GL_FRAMEBUFFER, 0);
         };

        void Show()
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, this->m_depthMap);
        };

        void Release()
        {
            glDeleteBuffers(1, &this->m_fBO);
            glDeleteBuffers(1, &this->m_depthMap);
        };

    private:
        unsigned int m_fBO;
        unsigned int m_depthMap;
};
#endif