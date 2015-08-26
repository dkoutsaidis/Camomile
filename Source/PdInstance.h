/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __CAMOMILE_PD_MASTER__
#define __CAMOMILE_PD_MASTER__

#include "PdAtom.h"

namespace pd
{
    class Patch;
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    //! @brief The manager for Patches.
    //! @details The Instance is a wrapper for the pd's native t_pdinstance.
    //! @details With an empty name, the Instance won't be initialized. A valid Instance
    //! @details should be created with a name. The Instance has some king of smart pointer
    //! @details behavior so when no Instance uses a t_pdinstance anymore the t_pdinstance
    //! @details is deleted. For the moment, several Instance can share the same name, but to
    //! @details ensure future compatibilty you should give a different name to all specifics
    //! @details Instances.
    class Instance
    {
        friend class Patch;
    private:
        struct Internal
        {
            t_pdinstance*       instance;
            std::mutex          mutex;
            std::set<Patch>     patcher;
            std::atomic<size_t> counter;
            std::string         name;
            
            Internal(std::string const& _name);
            ~Internal();
        };
        Internal*   m_internal;
    public:
        
        //! @brief The constructor for an empty Instance.
        //! @details Creates an Instance that can be used as an empty reference inside
        //! @details another class.
        Instance() noexcept;
        
        //! @brief The constructor for a new Instance.
        //! @details Creates a new valid Instance.
        Instance(std::string const& name) noexcept;
        
        //! @brief The copy constructor.
        //! @details Creates a copy of an Instance and increments his counter.
        Instance(Instance const& other) noexcept;
        
        //! @brief The move constructor.
        //! @details Creates a copy of an Instance and without incrementing his counter. The
        //! @details other instance will be useless.
        Instance(Instance&& other) noexcept;
        
        //! @brief The copy operator.
        //! @details Copies the Instance and increments his counter.
        Instance& operator=(Instance const& other) noexcept;
        
        //! @brief The move operator.
        //! @details Copies the Instance and without incrementing his counter. The other
        //! @details Instance will be destroyed if needed.
        Instance& operator=(Instance&& other) noexcept;
        
        //! @brief The destructor.
        //! @details The Instance will be destroyed if no other copy exists.
        ~Instance() noexcept;
        
        //! @brief Retrieves if the Instance is valid.
        inline operator bool() const noexcept {return bool(m_internal) && bool(m_internal->instance);}
        
        //! @brief Retrieves if the name of the Instance.
        inline std::string getName() const noexcept {return bool(m_internal) ? m_internal->name : std::string();}
        
        //! @brief Prepares the digital signal processing chain of the Instance.
        void prepareDsp(const int nins, const int nouts, const int samplerate, const int nsamples) noexcept;
        
        //! @brief Performs the digital signal processing chain of the Instance.
        void performDsp(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) noexcept;
        
        //! @brief Releases the digital signal processing chain of the Instance.
        void releaseDsp() noexcept;
        
        //! @brief Adds a path the global search path.
        static void addToSearchPath(std::string const& path) noexcept;
        
        //! @brief Clears all the paths of the global search path.
        static void clearSearchPath() noexcept;
        
    private:
        static std::mutex                       s_mutex;
        static int                              s_sample_rate;
        static const int                        s_max_channels = 16;
        static void print(const char* s) {std::cout << s << "\n";}
    };

}

#endif
