#pragma once

#include "zyre.h"
//#include "zyre_library.h" // for typedef of zyre_event_t
//#include "zyre_event.h"

#include <string>
#include <exception>
#include <vector>
#include <iostream>

namespace zyre
{

    class error_t : public std::runtime_error
    {
    public:
        error_t(const std::string& what) : std::runtime_error(what) {}
    };


    class zmsg
    {
    public:
        zmsg() : m_msg(zmsg_new()) {}
        zmsg(zmsg_t * msg)
        : m_msg(msg)
        {
        }
        ~zmsg()
        {
            if (m_msg)
            {
                if (is_zmsg()) zmsg_destroy(&m_msg);
            }
        }
        void add_str(std::string string)
        {
            zmsg_addstr(m_msg, string.c_str());
        }
        zmsg_t *msg_ptr()
        {
            if (!m_msg) std::cout << "WARNING: zmsg.msg_ptr() returned nullptr" << std::endl;
            return m_msg; 
        }
        size_t content_size()
        {
            if (m_msg) return zmsg_content_size(m_msg);
            std::cout << "WARNING:  zmsg_content_size() ran on a nullptr, returning 0" << std::endl;
            return 0;
        }
        size_t frame_count()
        {
            if (m_msg) return zmsg_size(m_msg);
            std::cout << "WARNING:  zmsg_size() ran on a nullptr, returning 0" << std::endl;
            return 0;
        }
        bool is_zmsg()
        {
            if (m_msg) return zmsg_is(m_msg);
            return false;
        }
        void nullify()
        {
            m_msg = nullptr;
        }

        void print()
        {
            if (!m_msg)
            {
                std::cout << "WARNING:  zmsg.print() called on a null zmsg_t*" << std::endl;
                return;
            }
            /*
            struct zyre_event_t {
                char *type;             //  Event type as string
                char *peer_uuid;        //  Sender UUID as string
                char *peer_name;        //  Sender public name as string
                char *peer_addr;        //  Sender ipaddress as string, for an ENTER event
                zhash_t *headers;       //  Headers, for an ENTER event
                char *group;            //  Group name for a SHOUT event
                zmsg_t *msg;            //  Message payload for SHOUT or WHISPER
            };
            */            

            std::vector<zframe_t *> frames;
            std::vector<int> frame_sizes;
            int count = frame_count();
            frames.resize(count);
            frame_sizes.resize(count);
            std::cout << "  There are " << count << " frames in the zmsg" << std::endl;
            for (int i = 0; i < count; i++)
            {
                frames.at(i) = zmsg_pop(m_msg);
                frame_sizes.at(i) = zframe_size(frames.at(i));
                std::cout << "    Frame " << i+1 << " size = " << frame_sizes.at(i) << " bytes" << std::endl; 
            }

            const char *pZType = (char*)zframe_data(frames.at(0));
            const char *pPeer = (char*)zframe_data(frames.at(1));
            const char *pName = (char*)zframe_data(frames.at(2));                    
            std::string ztype;
            std::string peer;
            std::string name;
            if (pZType)
            {
                ztype = std::string(pZType);
                ztype = ztype.substr(0, frame_sizes.at(0)); 
                std::cout << "  zmsg type = " << ztype << std::endl;               
            }
            if (pPeer)
            {
                peer = std::string(pPeer);
                peer = peer.substr(0, frame_sizes.at(1));
                std::cout << "  peer UUID = " << peer << std::endl;
            }
            if (pName)
            {
                name = std::string(pName);
                name = name.substr(0, frame_sizes.at(2));
                std::cout << "  peer name = " << name << std::endl;
            }

            // do special things based on the type
            if (ztype.compare("ENTER") == 0)
            {
                std::string addr;
                std::string headers;
                const char *pHeaders = (char*)zframe_data(frames.at(3));
                const char *pAddr = (char*)zframe_data(frames.at(4));
                if (pHeaders)
                {
                    headers = std::string(pHeaders);
                    headers = headers.substr(0, frame_sizes.at(3));
                    std::cout << "  headers = " << headers << std::endl;
                }                
                if (pAddr)
                {
                    addr = std::string(pAddr);
                    addr = addr.substr(0, frame_sizes.at(4));
                    std::cout << "  peer addr = " << addr << std::endl;
                }
            }
            if (ztype.compare("JOIN") == 0)
            {
                std::string group;
                const char *pGroup = (char*)zframe_data(frames.at(3));
                if (pGroup)
                {
                    group = std::string(pGroup);
                    group = group.substr(0, frame_sizes.at(3));
                    std::cout << "  peer group = " << group << std::endl;
                }
            }
            if (ztype.compare("SHOUT") == 0)
            {
                std::string group;
                std::string message;
                const char *pGroup = (char*)zframe_data(frames.at(3));
                const char *pMsg = (char*)zframe_data(frames.at(4));
                if (pGroup)
                {
                    group = std::string(pGroup);
                    group = group.substr(0, frame_sizes.at(3));
                    std::cout << "  peer group = " << group << std::endl;
                }                                  
                if (pMsg)
                {
                    message = std::string(pMsg);
                    message = message.substr(0, frame_sizes.at(4));
                    std::cout << "  message = " << message << std::endl;
                }
            }
            if (ztype.compare("WHISPER") == 0)
            {
                std::string message;
                const char *pMsg = (char*)zframe_data(frames.at(3));
                if (pMsg)
                {
                    message = std::string(pMsg);
                    message = message.substr(0, frame_sizes.at(3));
                    std::cout << "  message = " << message << std::endl;
                }                
            }
            if (ztype.compare("EXIT") == 0)
            {
                // TODO remove the peer from any lists
            }

            // destroy all the frames
            for (int i = 0; i < count; i++)
            {
                zframe_destroy(&frames.at(i));
            }
        }
    private:
        zmsg_t *m_msg = nullptr;
    };


    class event_t
    {
    public:
        event_t(zyre_event_t* self) : m_self(self) {};        

        ~event_t()
        {
            if (m_self) zyre_event_destroy(&m_self);
        }

        event_t(const event_t& other) = delete;
        event_t operator=(const event_t& other) = delete;

        event_t(event_t&& other)
        {
            m_self = other.m_self;
            other.m_self = nullptr;
        }

        event_t& operator=(event_t&& other)
        {
            if (&other != this)
            {
                m_self = other.m_self;
                other.m_self = nullptr;
            }
            return *this;
        }

        void print() const
        {
            zyre_event_print(m_self);
        }

        std::string type() const
        {
            const char *val = zyre_event_type(m_self);
            if (val == NULL) return "";
            return val;
        }

        std::string sender() const
        {
            const char *val = zyre_event_peer_uuid(m_self);
            if (val == NULL) return "";
            return val;
        }

        std::string name() const
        {
            const char *val = zyre_event_peer_name(m_self);
            if(val == NULL) return "";
            return val;
        }

        std::string address() const
        {
            const char *val = zyre_event_peer_addr(m_self);
            if(val == NULL) return "";
            return val;
        }

        std::string header_value(const std::string& key) const
        {
            const char *val = zyre_event_header(m_self, key.c_str());
            if(val == NULL) return "";
            return val;
        }

        std::string group() const
        {
            const char *val = zyre_event_group(m_self);
            if(val == NULL) return "";
            return val;
        }

        zmsg_t* message() const
        {
            return zyre_event_msg(m_self);
        }

    private:
        zyre_event_t* m_self;
    };

    class node_t
    {
    public:
        node_t(const std::string& name = "")
        {
            if (name == "") m_self = zyre_new(NULL);
            else m_self = zyre_new(name.c_str());

            m_poller = zpoller_new(socket(), nullptr); // need the last argument to be a nullptr!
        }

        ~node_t()
        {
            if (m_self) zyre_destroy(&m_self);
            if (m_poller) zpoller_destroy(&m_poller);
        }

        node_t(const node_t& other) = delete;
        node_t operator=(const node_t& other) = delete;

        node_t(node_t&& other)
        {
            m_self = other.m_self;
            other.m_self = nullptr;
        }

        node_t& operator=(node_t&& other)
        {
            if (&other != this)
            {
                m_self = other.m_self;
                other.m_self = nullptr;
            }
            return *this;
        }

        void print() const
        {
            zyre_print(m_self);
        }

        std::string uuid() const
        {
            return zyre_uuid(m_self);
        }

        std::string name() const
        {
            return zyre_name(m_self);
        }

        void set_header(const std::string key, const std::string& value) const
        {
            zyre_set_header(m_self, key.c_str(), "%s", value.c_str());
        }

        void set_verbose() const
        {
            zyre_set_verbose(m_self);
        }

        void set_port(int value) const
        {
            zyre_set_port(m_self, value);
        }

        void set_interval(size_t value) const
        {
            zyre_set_interval(m_self, value);
        }

        void set_interface(const std::string& value) const
        {
            zyre_set_interface(m_self, value.c_str());
        }

        void set_evasive_timeout(int msec)
        {
            zyre_set_evasive_timeout (m_self, msec);
        }
        void set_expired_timeout(int msec)
        {
            zyre_set_expired_timeout (m_self, msec);
        }   

        void start() const
        {
            int rc = zyre_start(m_self);
            if (rc == -1) throw error_t("Failed to start Zyre node");
        }

        void stop() const
        {
            zyre_stop(m_self);
        }

        void join(const std::string& group) const
        {
            zyre_join(m_self, group.c_str());
        }

        void leave(const std::string& group) const
        {
            zyre_leave(m_self, group.c_str());
        }

        void whisper(const std::string& peer, zmsg_t* msg) const
        {
            zyre_whisper(m_self, peer.c_str(), &msg);
        }

        void shout(const std::string& group, zmsg_t* msg) const
        {
            zyre_shout(m_self, group.c_str(), &msg);
        }

        zmsg_t* recv() const
        {
            return zyre_recv(m_self);
        }

        zmsg_t *recv_deadline(int msec_deadline) const
        {
            //std::cout << "node_t::recv_deadline()..." << std::endl;
            if (!m_poller)
            {
                std::cout << "WARNING: zpoller is null" << std::endl;
                return nullptr;
            }
            void *which = zpoller_wait(m_poller, msec_deadline);
            if (which) return zmsg_recv(which);
            return nullptr;
        }

        bool poller_expired()
        {
            if (m_poller) return zpoller_expired(m_poller);
            return false;
        }

        event_t event() const
        {
            return event_t(zyre_event_new(m_self));
        }

        std::vector<std::string> peers() const
        {
            zlist_t* peers = zyre_peers(m_self);
            std::vector<std::string> ret;
            if(peers == NULL) {
                return ret;
            }
            ret = to_vector(peers);
            zlist_destroy(&peers);
            return ret;
        }

        std::vector<std::string> own_groups() const
        {
            zlist_t* ownGroups = zyre_own_groups(m_self);
            std::vector<std::string> ret;
            if(ownGroups == NULL) {
                return ret;
            }
            ret = to_vector(ownGroups);
            zlist_destroy(&ownGroups);
            return ret;
        }

        std::vector<std::string> peer_groups() const
        {
            zlist_t* peerGroups = zyre_peer_groups(m_self);
            std::vector<std::string> ret;
            if(peerGroups == NULL) {
                return ret;
            }
            ret = to_vector(peerGroups);
            zlist_destroy(&peerGroups);
            return ret;
        }

        std::string peer_address(const std::string& peer) const
        {
            char* val = zyre_peer_address(m_self, peer.c_str());
            std::string ret(val);
            if (val != NULL)
                delete val;
            return ret;
        }

        std::string peer_header_value(const std::string& peer, const std::string& name)
        {
            char* val = zyre_peer_header_value(m_self, peer.c_str(), peer.c_str());
            std::string ret(val);
            if (val != NULL)
                delete val;
            return ret;
        }

        zsock_t* socket() const
        {
            return zyre_socket(m_self);
        }

        static int version()
        {
            return zyre_version();
        }

    private:
        std::vector<std::string> to_vector(zlist_t* list) const
        {
            std::vector<std::string> ret;
            void* cursor = zlist_first(list);
            while (cursor != NULL)
            {
                ret.emplace_back(static_cast<char*>(cursor));
                cursor = zlist_next(list);
            }
            return ret;
        }

        zyre_t *m_self = nullptr;
        zpoller_t *m_poller = nullptr;
    };
}
