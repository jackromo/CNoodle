# Todo

## Features

* Apply concurrency with pthreads
    * Make thread pool for update / render (see ThreadPoolC repo)
    * Add locks to linked lists in hashtables
    * Make acquire_lock / is_locked / release_lock funcs
        * Given an id, get lock for its linked list
        * Methods try lock 10 times, return bool of success
        * Special versions for entities, rooms, etc.
    * Use locks in dispatchers / rendering only
* Rendering (OpenGL)
    * make_sprite, load_sprite and draw_sprite methods
    * Fill in render loop with thread pool
        * Draw relative to camera position
    * Add views (?)
* Audio (PortAudio)
    * make_sound, file path for audio -> t_sound
    * play_snd, pause_snd, stop_snd dispatchers
* Entity
    * Template update methods
    * Helper functions, hide game_data access
        * Also to auto-generate update commands and container
    * Physics (hitboxes, pixel-perfect collision, hit-circles, etc.)
* Add tests for everything
    * use glib unit testing facilities
    * tests directory, capability to build with tests
    * ability to choose which tests to run
* Replace llist with GSList, llist is pointless


## Bugfixes

* Fix command dispatchers
    * Many data types now use IDs rather than values
    * Elements passed to gamedata by pointer now
* Test makefile 'test' directive
