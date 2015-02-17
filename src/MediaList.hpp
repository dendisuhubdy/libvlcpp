/*****************************************************************************
 * libvlc_MediaList.hpp: MediaList API
 *****************************************************************************
 * Copyright © 2014 the VideoLAN team
 *
 * Authors: Alexey Sokolov <alexey@alexeysokolov.co.cc>
 *          Hugo Beauzée-Luyssen <hugo@beauzee.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef LIBVLC_CXX_MEDIALIST_H
#define LIBVLC_CXX_MEDIALIST_H

#include "common.hpp"

namespace VLC
{

class Media;
class EventManager;
class MediaDiscoverer;
class MediaLibrary;

class MediaList : public Internal<libvlc_media_list_t>
{
public:
    /**
     * Check if 2 MediaList objects contain the same libvlc_media_list_t.
     * \param another another MediaList
     * \return true if they contain the same libvlc_media_list_t
     */
    bool operator==(const MediaList& another) const
    {
        return m_obj == another.m_obj;
    }

    // libvlc_media_subitems
    /**
     * Get subitems of media descriptor object. This will increment the
     * reference count of supplied media descriptor object. Use
     * MediaList::release() to decrement the reference counting.
     *
     * \param p_md  media descriptor object
     */
    MediaList(Media& md)
        : Internal{ libvlc_media_subitems( getInternalPtr<libvlc_media_t>( md ) ), libvlc_media_list_release }
    {
    }

    // libvlc_media_discoverer_media_list
    /**
     * Get media service discover media list.
     *
     * \param p_mdis  media service discover object
     */
    MediaList(MediaDiscoverer& mdis)
        : Internal{ libvlc_media_discoverer_media_list( getInternalPtr<libvlc_media_discoverer_t>( mdis ) ),
                    libvlc_media_list_release }
    {
    }

    // libvlc_media_library_media_list
    /**
     * Get media library subitems.
     *
     * \param p_mlib  media library object
     */
    MediaList(MediaLibrary& mlib )
        : Internal{ libvlc_media_library_media_list( getInternalPtr<libvlc_media_library_t>( mlib ) ), libvlc_media_list_release }
    {
    }


    // libvlc_media_list_new
    /**
     * Create an empty media list.
     *
     * \param p_instance  libvlc instance
     */
    MediaList(Instance& instance)
        : Internal{ libvlc_media_list_new( getInternalPtr<libvlc_instance_t>( instance ) ),
                                           libvlc_media_list_release }
    {
    }

    /**
     * Associate media instance with this media list instance. If another
     * media instance was present it will be released. The
     * libvlc_media_list_lock should NOT be held upon entering this function.
     *
     * \param p_md  media instance to add
     */
    void setMedia(Media& md)
    {
        libvlc_media_list_set_media( *this, getInternalPtr<libvlc_media_t>( md ) );
    }

    /**
     * Add media instance to media list The libvlc_media_list_lock should be
     * held upon entering this function.
     *
     * \param p_md  a media instance
     *
     * \return 0 on success, -1 if the media list is read-only
     */
    int addMedia(Media& md)
    {
        return libvlc_media_list_add_media( *this, getInternalPtr<libvlc_media_t>( md ) );
    }

    /**
     * Insert media instance in media list on a position The
     * libvlc_media_list_lock should be held upon entering this function.
     *
     * \param p_md  a media instance
     *
     * \param i_pos  position in array where to insert
     *
     * \return 0 on success, -1 if the media list is read-only
     */
    int insertMedia(Media& md, int pos)
    {
        return libvlc_media_list_insert_media( *this, getInternalPtr<libvlc_media_t>( md ), pos );
    }

    /**
     * Remove media instance from media list on a position The
     * libvlc_media_list_lock should be held upon entering this function.
     *
     * \param i_pos  position in array where to insert
     *
     * \return 0 on success, -1 if the list is read-only or the item was not
     * found
     */
    int removeIndex(int i_pos)
    {
        return libvlc_media_list_remove_index(*this,i_pos);
    }

    /**
     * Get count on media list items The libvlc_media_list_lock should be
     * held upon entering this function.
     *
     * \return number of items in media list
     */
    int count()
    {
        return libvlc_media_list_count(*this);
    }

    /**
     * List media instance in media list at a position The
     * libvlc_media_list_lock should be held upon entering this function.
     *
     * \param i_pos  position in array where to insert
     *
     * \return media instance at position i_pos, or NULL if not found. In
     * case of success, Media::retain() is called to increase the refcount on
     * the media.
     */
    MediaPtr itemAtIndex(int i_pos)
    {
        auto ptr = libvlc_media_list_item_at_index(*this,i_pos);
        return std::make_shared<Media>( ptr, false );
    }

    /**
     * Find index position of List media instance in media list. Warning: the
     * function will return the first matched position. The
     * libvlc_media_list_lock should be held upon entering this function.
     *
     * \param p_md  media instance
     *
     * \return position of media instance or -1 if media not found
     */
    int indexOfItem(Media& md)
    {
        return libvlc_media_list_index_of_item( *this, getInternalPtr<libvlc_media_t>( md ) );
    }

    /**
     * This indicates if this media list is read-only from a user point of
     * view
     *
     * \return 1 on readonly, 0 on readwrite
     */
    bool isReadonly()
    {
        return libvlc_media_list_is_readonly(*this);
    }

    /**
     * Get lock on media list items
     */
    void lock()
    {
        libvlc_media_list_lock(*this);
    }

    /**
     * Release lock on media list items The libvlc_media_list_lock should be
     * held upon entering this function.
     */
    void unlock()
    {
        libvlc_media_list_unlock(*this);
    }

    /**
     * Get libvlc_event_manager from this media list instance. The
     * p_event_manager is immutable, so you don't have to hold the lock
     *
     * \return libvlc_event_manager
     */
    EventManagerPtr eventManager()
    {
        if ( m_eventManager )
        {
            libvlc_event_manager_t* obj = libvlc_media_list_event_manager( *this );
            m_eventManager = std::make_shared<EventManager>( obj );
        }
        return m_eventManager;
    }

private:
    EventManagerPtr m_eventManager;
};

} // namespace VLC

#endif

