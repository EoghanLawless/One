/*
	------------------------------------------------------------------------------
		Licensing information can be found at the end of the file.
	------------------------------------------------------------------------------

	cute_sound.h - v1.09

	To create implementation (the function definitions)
		#define CUTE_SOUND_IMPLEMENTATION
	in *one* C/CPP file (translation unit) that includes this file

	Summary:
	cute_sound is a C API for loading, playing, looping, panning and fading mono
	and stero sounds. This means cute_sound imparts no external DLLs or large
	libraries that adversely effect shipping size. cute_sound can also run on
	Windows XP since DirectSound ships with all recent versions of Windows.
	cute_sound implements a custom SSE2 mixer. cute_sound uses CoreAudio for Apple
	machines (like OSX and iOS). SDL is used for all other platforms. Define
	CUTE_SOUND_FORCE_SDL before placaing the CUTE_SOUND_IMPLEMENTATION in order to force
	the use of SDL.

	Revision history:
		1.0  (06/04/2016) initial release
		1.01 (06/06/2016) load WAV from memory
		                  separate portable and OS-specific code in cs_mix
		                  fixed bug causing audio glitches when sounds ended
		                  added stb_vorbis loaders + demo example
		1.02 (06/08/2016) error checking + strings in vorbis loaders
		                  SSE2 implementation of mixer
		                  fix typos on docs/comments
		                  corrected volume bug introduced in 1.01
		1.03 (07/05/2016) size calculation helper (to know size of sound in
		                  bytes on the heap) cs_sound_size
		1.04 (12/06/2016) merged in Aaron Balint's contributions
		                  SFFT and pitch functions from Stephan M. Bernsee
		                  cs_mix can run on its own thread with cs_spawn_mix_thread
		                  updated documentation, typo fixes
		                  fixed typo in cs_malloc16 that caused heap corruption
		1.05 (12/08/2016) cs_stop_all_sounds, suggested by Aaron Balint
		1.06 (02/17/2017) port to CoreAudio for Apple machines
		1.07 (06/18/2017) SIMD the pitch shift code; swapped out old Bernsee
		                  code for a new re-write, updated docs as necessary,
		                  support for compiling as .c and .cpp on Windows,
		                  port for SDL (for Linux, or any other platform).
		                  Special thanks to DeXP (Dmitry Hrabrov) for 90% of
		                  the work on the SDL port!
		1.08 (09/06/2017) SDL_RWops support by RobLoach
		1.09 (05/20/2018) Load wav funcs can skip all irrelevant chunks
		                  Ref counting for playing sounds
*/

/*
	Contributors:
		Aaron Balint      1.04 - real time pitch
		                  1.04 - separate thread for cs_mix
		                  1.04 - bugfix, removed extra cs_free16 call for second channel
		DeXP              1.07 - initial work on SDL port
		RobLoach          1.08 - SDL_RWops support
*/

/*
	DOCUMENTATION (very quick intro):
	1. create context
	2. load sounds from disk into memory
	3. play sounds
	4. free context

	1. cs_context_t* ctx = cs_make_context(hwnd, frequency, latency, seconds, N);
	2. cs_play_sound_def_t def = cs_make_def(&cs_load_wav("path_to_file/filename.wav"));
	3. cs_play_sound(ctx, def);
	4. cs_shutdown_context(ctx);

	DOCUMENTATION (longer introduction):
	cute_sound consists of cs_loaded_sound_t, cs_playing_sound_t and the cs_context_t.
	The cs_context_t encapsulates an OS sound API, as well as buffers + settings.
	cs_loaded_sound_t holds raw samples of a sound. cs_playing_sound_t is an instance
	of a cs_loaded_sound_t that represents a sound that can be played through the
	cs_context_t.

	There are two main versions of the API, the low-level and the high-level
	API. The low-level API does not manage any memory for cs_playing_sound_ts. The
	high level api holds a memory pool of playing sounds.

	To actually mix sounds together and send audio to the sound card, be sure
	to call either cs_mix periodically or call cs_spawn_mix_thread once.

	High-level API:
		First create a context and pass in non-zero to the final parameter. This
		final parameter controls how large of a memory pool to use for cs_playing_sound_ts.
		Here's an example where N is the size of the internal pool:

		cs_context_t* ctx = cs_make_context(hwnd, frequency, latency, seconds, N);

		We create cs_playing_sound_ts indirectly with tsPlayDef structs. tsPlayDef is a
		POD struct so feel free to make them straight on the stack. The tsPlayDef
		sets up initialization parameters. Here's an example to load a wav and
		play it:

		cs_loaded_sound_t loaded = cs_load_wav("path_to_file/filename.wav");
		cs_play_sound_def_t def = cs_make_def(&loaded);
		cs_playing_sound_t* sound = cs_play_sound(ctx, def);

		The same def can be used to play as many sounds as desired (even simultaneously)
		as long as the context playing sound pool is large enough.

	Low-level API:
		First create a context and pass 0 in the final parameter (0 here means
		the context will *not* allocate a cs_playing_sound_t memory pool):

		cs_context_t* ctx = cs_make_context(hwnd, frequency, latency, seconds, 0);

		parameters:
			hwnd           --  HWND, handle to window (on OSX just pass in 0)
			frequency      --  int, represents Hz frequency rate in which samples are played
			latency        --  int, estimated latency in Hz from PlaySound call to speaker output
			seconds        --  int, number of second of samples internal buffers can hold
			0 (last param) --  int, number of elements in cs_playing_sound_t pool

		We create a cs_playing_sound_t like so:
		cs_loaded_sound_t loaded = cs_load_wav("path_to_file/filename.wav");
		cs_playing_sound_t playing_sound = cs_make_playing_sound(&loaded);

		Then to play the sound we do:
		cs_insert_sound(ctx, &playing_sound);

		The above cs_insert_sound function call will place playing_sound into
		a singly-linked list inside the context. The context will remove
		the sound from its internal list when it finishes playing.

	WARNING: The high-level API cannot be mixed with the low-level API. If you
	try then the internal code will assert and crash. Pick one and stick with it.
	Usually he high-level API will be used, but if someone is *really* picky about
	their memory usage, or wants more control, the low-level API can be used.

	Here is the Low-Level API:
		cs_playing_sound_t cs_make_playing_sound(cs_loaded_sound_t* loaded);
		int cs_insert_sound(cs_context_t* ctx, cs_playing_sound_t* sound);

	Here is the High-Level API:
		cs_playing_sound_t* cs_play_sound(cs_context_t* ctx, cs_play_sound_def_t def);
		cs_play_sound_def_t cs_make_def(cs_loaded_sound_t* sound);
		void cs_stop_all_sounds(cs_context_t(ctx);

	Be sure to link against dsound.dll (or dsound.lib) on Windows.

	Read the rest of the header for specific details on all available functions
	and struct types.
*/

/*
	Known Limitations:

	* PCM mono/stereo format is the only formats the LoadWAV function supports. I don't
		guarantee it will work for all kinds of wav files, but it certainly does for the common
		kind (and can be changed fairly easily if someone wanted to extend it).
	* Only supports 16 bits per sample.
	* Mixer does not do any fancy clipping. The algorithm is to convert all 16 bit samples
		to float, mix all samples, and write back to audio API as 16 bit integers. In
		practice this works very well and clipping is not often a big problem.
	* I'm not super familiar with good ways to avoid the DirectSound play cursor from going
		past the write cursor. To mitigate this pass in a larger number to cs_make_context's 4th
		parameter (buffer scale in seconds).
	* Pitch shifting code is pretty darn expensive. This is due to the use of a Fast Fourier Transform
		routine. The pitch shifting itself is written in rather efficient SIMD using SSE2 intrinsics,
		but the FFT routine is very basic. FFT is a big bottleneck for pitch shifting. There is a
		TODO optimization listed in this file for the FFT routine, but it's fairly low priority;
		optimizing FFT routines is difficult and requires a lot of specialized knowledge.
*/

/*
	FAQ
	Q : Why DirectSound instead of (insert API here) on Windows?
	A : Casey Muratori documented DS on Handmade Hero, other APIs do not have such good docs. DS has
	shipped on Windows XP all the way through Windows 10 -- using this header effectively intro-
	duces zero dependencies for the foreseeable future. The DS API itself is sane enough to quickly
	implement needed features, and users won't hear the difference between various APIs. Latency is
	not that great with DS but it is shippable. Additionally, many other APIs will in the end speak
	to Windows through the DS API.

	Q : Why not include Linux support?
	A : There have been a couple requests for ALSA support on Linux. For now the only option is to use
	SDL backend, which can indirectly support ALSA. SDL is used only in a very low-level manner;
	to get sound samples to the sound card via callback, so there shouldn't be much in the way of
	considering SDL a good option for "name your flavor" of Linux backend.

	Q : I would like to use my own memory management, how can I achieve this?
	A : This header makes a couple uses of malloc/free, and cs_malloc16/cs_free16. Simply find these bits
	and replace them with your own memory allocation routines. They can be wrapped up into a macro,
	or call your own functions directly -- it's up to you. Generally these functions allocate fairly
	large chunks of memory, and not very often (if at all), with one exception: cs_set_pitch is a very
	expensive routine and requires frequent dynamic memory management.

	Q : Does this library support audio streaming? Something like System::createStream in FMOD.
	A : No. Typically music files aren't that large (in the megabytes). Compare this to a typical
	DXT texture of 1024x1024, at 0.5MB of memory. Now say an average music file for a game is three
	minutes long. Loading this file into memory and storing it as raw 16bit samples with two channels,
	would be:

		num_samples = 3 * 60 * 44100 * 2
		num_bits = num_samples * 16
		num_bytes = num_bits / 8
		num_megabytes = num_bytes / (1024 * 1024)
		or 30.3mb

	So say the user has 2gb of spare RAM. That means we could fit 67 different three minute length
	music files in there simultaneously. That is a ridiculous amount of spare memory. 30mb is nothing
	nowadays. Just load your music file into memory all at once and then play it.

	Q : But I really need streaming of audio files from disk to save memory! Also loading my audio
	files (like .OGG) takes a long time (multiple seconds).
	A : It is recommended to either A) load up your music files before they are needed, perhaps during
	a loading screen, or B) stream in the entire audio into memory on another thread. cs_read_mem_ogg is
	a great candidate function to throw onto a job pool. Streaming is more a remnant of older machines
	(like in the 90's or early 2000's) where decoding speed and RAM were real nasty bottlenecks. For
	more modern machines, these aren't really concerns, even with mobile devices. If even after reading
	this Q/A section you still want to stream your audio, you can try mini_al as an alternative:
	https://github.com/dr-soft/mini_al
*/

/*
	Some past discussion threads:
	https://www.reddit.com/r/gamedev/comments/6i39j2/tinysound_the_cutest_library_to_get_audio_into/
	https://www.reddit.com/r/gamedev/comments/4ml6l9/tinysound_singlefile_c_audio_library/
	https://forums.tigsource.com/index.php?topic=58706.0
*/



#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <dsound.h>
#undef PlaySound

#ifndef CUTE_SOUND_ALLOC
#include <stdlib.h>
#define CUTE_SOUND_ALLOC(size) malloc(size)
#define CUTE_SOUND_FREE(mem) free(mem)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifndef _WAVEFORMATEX_
#include <mmreg.h>
#endif


#if defined(_MSC_VER)
#pragma comment(lib, "dsound.lib")
#endif

#define CUTE_SOUND_CHECK(X, Y) do { if (!(X)) { cs_error_reason = Y; goto ts_err; } } while (0)
#define CUTE_SOUND_ASSERT_INTERNAL *(int*)0 = 0
#define CUTE_SOUND_ASSERT(X) do { if (!(X)) CUTE_SOUND_ASSERT_INTERNAL; } while (0)
#define CUTE_SOUND_ALIGN(X, Y) ((((size_t)X) + ((Y) - 1)) & ~((Y) - 1))
#define CUTE_SOUND_TRUNC(X, Y) ((size_t)(X) & ~((Y) - 1))

#define CUTE_SOUND_MAX_FRAME_LENGTH 4096
#define CUTE_SOUND_PITCH_FRAME_SIZE 512
#define CUTE_SOUND_PITCH_QUALITY 8

#define CUTE_SOUND_STEPSIZE (CUTE_SOUND_PITCH_FRAME_SIZE / CUTE_SOUND_PITCH_QUALITY)
#define CUTE_SOUND_OVERLAP (CUTE_SOUND_PITCH_FRAME_SIZE - CUTE_SOUND_STEPSIZE)
#define CUTE_SOUND_EXPECTED_FREQUENCY (2.0f * 3.14159265359f * (float)CUTE_SOUND_STEPSIZE / (float)CUTE_SOUND_PITCH_FRAME_SIZE)
#define CUTE_SOUND_ALIGN16_0 __declspec(align(16))
#define CUTE_SOUND_ALIGN16_1

#define tsVonHann(i) (-0.5f * cosf(2.0f * 3.14159265359f * (float)(i) / (float)CUTE_SOUND_PITCH_FRAME_SIZE) + 0.5f)


#define _PS_CONST_TYPE(Name, Type, Val) \
	CUTE_SOUND_ALIGN16_0 Type _ps_##Name[4] CUTE_SOUND_ALIGN16_1 = { Val, Val, Val, Val }
#define _PS_CONST(Name, Val) \
	CUTE_SOUND_ALIGN16_0 float _ps_##Name[4] CUTE_SOUND_ALIGN16_1 = { Val, Val, Val, Val }
#define _PI32_CONST(Name, Val) \
	CUTE_SOUND_ALIGN16_0 int _pi32_##Name[4] CUTE_SOUND_ALIGN16_1 = { Val, Val, Val, Val }

inline _PS_CONST_TYPE(sign_mask, int, (int)0x80000000);
inline _PS_CONST_TYPE(inv_sign_mask, int, (int)~0x80000000);

inline _PS_CONST(atanrange_hi, 2.414213562373095f);
inline _PS_CONST(atanrange_lo, 0.4142135623730950f);
inline _PS_CONST(cephes_PIO2F, 1.5707963267948966192f);
inline _PS_CONST(cephes_PIO4F, 0.7853981633974483096f);
inline _PS_CONST(1, 1.0f);
inline _PS_CONST(0p5, 0.5f);
inline _PS_CONST(0, 0);
inline _PS_CONST(sincof_p0, -1.9515295891E-4f);
inline _PS_CONST(sincof_p1, 8.3321608736E-3f);
inline _PS_CONST(sincof_p2, -1.6666654611E-1f);
inline _PS_CONST(atancof_p0, 8.05374449538e-2f);
inline _PS_CONST(atancof_p1, 1.38776856032E-1f);
inline _PS_CONST(atancof_p2, 1.99777106478E-1f);
inline _PS_CONST(atancof_p3, 3.33329491539E-1f);
inline _PS_CONST(cephes_PIF, 3.141592653589793238f);
inline _PS_CONST(cephes_2PIF, 2.0f * 3.141592653589793238f);
inline _PS_CONST(cephes_FOPI, 1.27323954473516f); // 4 / M_PI
inline _PS_CONST(minus_cephes_DP1, -0.78515625f);
inline _PS_CONST(minus_cephes_DP2, -2.4187564849853515625e-4f);
inline _PS_CONST(minus_cephes_DP3, -3.77489497744594108e-8f);
inline _PS_CONST(coscof_p0, 2.443315711809948E-005f);
inline _PS_CONST(coscof_p1, -1.388731625493765E-003f);
inline _PS_CONST(coscof_p2, 4.166664568298827E-002f);
inline _PS_CONST(frame_size, (float)CUTE_SOUND_PITCH_FRAME_SIZE);

inline _PI32_CONST(1, 1);
inline _PI32_CONST(inv1, ~1);
inline _PI32_CONST(2, 2);
inline _PI32_CONST(4, 4);


#define CUTE_SOUND_WINDOWS	1
#define CUTE_SOUND_MAC		2
#define CUTE_SOUND_UNIX		3
#define CUTE_SOUND_SDL		4

#if defined(_WIN32)

	#define CUTE_SOUND_PLATFORM CUTE_SOUND_WINDOWS

	#if !defined _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#endif

#endif

// read this in the event of cs_load_wav/cs_load_ogg errors
// also read this in the event of certain errors from cs_make_context
extern const char* cs_error_reason;

// stores a loaded sound in memory
typedef struct cs_loaded_sound_t
{
	int sample_rate;
	int sample_count;
	int channel_count;

	// Number of instances currently referencing this audio. Must be zero
	// in order to safely delete the audio. References are automatically
	// updated whenever playing instances are inserted into the context.
	int playing_count;

	// The actual raw audio samples in memory.
	void* channels[2];
} cs_loaded_sound_t;

struct cs_pitch_data_t;

typedef struct cs_pitch_data_t cs_pitch_data_t;

// represents an instance of a cs_loaded_sound_t, can be played through the cs_context_t
typedef struct cs_playing_sound_t
{
	int active;
	int paused;
	int looped;
	float volume0;
	float volume1;
	float pan0;
	float pan1;
	float pitch;
	cs_pitch_data_t* pitch_filter[2];
	int sample_index;
	cs_loaded_sound_t* loaded_sound;
	struct cs_playing_sound_t* next;
} cs_playing_sound_t;

// holds audio API info and other info
struct cs_context_t;

typedef struct cs_context_t cs_context_t;

// The returned struct will contain a null pointer in cs_loaded_sound_t::channel[0]
// in the case of errors. Read cs_error_reason string for details on what happened.
// Calls cs_read_mem_wav internally.
cs_loaded_sound_t cs_load_wav(const char* path);

// Reads a WAV file from memory. Still allocates memory for the cs_loaded_sound_t since
// WAV format will interlace stereo, and we need separate data streams to do SIMD
// properly.
void cs_read_mem_wav(const void* memory, int size, cs_loaded_sound_t* sound);

// Uses cs_free16 (aligned free, implemented later in this file) to free up both of
// the channels stored within sound
void cs_free_sound(cs_loaded_sound_t* sound);

// Returns the size, in bytes, of all heap-allocated memory for this particular
// loaded sound
int cs_sound_size(cs_loaded_sound_t* sound);

// playing_pool_count -- 0 to setup low-level API, non-zero to size the internal
// memory pool for cs_playing_sound_t instances
cs_context_t* cs_make_context(void* hwnd, unsigned play_frequency_in_Hz, int latency_factor_in_Hz, int num_buffered_seconds, int playing_pool_count);

void cs_shutdown_context(cs_context_t* ctx);

// Call cs_spawn_mix_thread once to setup a separate thread for the context to run
// upon. The separate thread will continually call cs_mix and perform mixing
// operations.
void cs_spawn_mix_thread(cs_context_t* ctx);

// Use cs_thread_sleep_delay to specify a custom sleep delay time.
// A sleep will occur after each call to cs_mix. By default YieldProcessor
// is used, and no sleep occurs. Use a sleep delay to conserve CPU bandwidth.
// A recommended sleep time is a little less than 1/2 your predicted 1/FPS.
// 60 fps is 16 ms, so about 1-5 should work well in most cases.
void cs_thread_sleep_delay(cs_context_t* ctx, int milliseconds);

// Call this manually, once per game tick recommended, if you haven't ever
// called cs_spawn_mix_thread. Otherwise the thread will call cs_mix itself.
// num_samples_to_write is not used on Windows. On Mac it is used to push
// samples into a circular buffer while CoreAudio simultaneously pulls samples
// off of the buffer. num_samples_to_write should be computed each update tick
// as delta_time * play_frequency_in_Hz + 1.
void cs_mix(cs_context_t* ctx);

// All of the functions in this next section should only be called if cs_is_active
// returns true. Calling them otherwise probably won't do anything bad, but it
// won't do anything at all. If a sound is active it resides in the context's
// internal list of playing sounds.
int cs_is_active(cs_playing_sound_t* sound);

// Flags sound for removal. Upon next cs_mix call will remove sound from playing
// list. If high-level API used sound is placed onto the internal free list.
void cs_stop_sound(cs_playing_sound_t* sound);

void cs_loop_sound(cs_playing_sound_t* sound, int zero_for_no_loop);

void cs_pause_sound(cs_playing_sound_t* sound, int one_for_paused);

// lerp from 0 to 1, 0 full left, 1 full right
void cs_set_pan(cs_playing_sound_t* sound, float pan);

// explicitly set volume of each channel. Can be used as panning (but it's
// recommended to use the cs_set_pan function for panning).
void cs_set_volume(cs_playing_sound_t* sound, float volume_left, float volume_right);

// Change pitch (not duration) of sound. pitch = 0.5f for one octave lower, pitch = 2.0f for one octave higher.
// pitch at 1.0f applies no change. pitch settings farther away from 1.0f create more distortion and lower
// the output sample quality. pitch can be adjusted in real-time for doppler effects and the like. Going beyond
// 0.5f and 2.0f may require some tweaking the pitch shifting parameters, and is not recommended.
//
// Additional important information about performance: This function
// is quite expensive -- you have been warned! Try it out and be aware of how much CPU consumption it uses.
// To avoid destroying the originally loaded sound samples, cs_set_pitch will do a one-time allocation to copy
// sound samples into a new buffer. The new buffer contains the pitch adjusted samples, and these will be played
// through cs_mix. This lets the pitch be modulated at run-time, but requires dynamically allocated memory. The
// memory is freed once the sound finishes playing. If a one-time pitch adjustment is desired, for performance
// reasons please consider doing an off-line pitch adjustment manually as a pre-processing step for your sounds.
// Also, consider changing cs_malloc16 and cs_free16 to match your custom memory allocation needs. Try adjusting
// CUTE_SOUND_PITCH_QUALITY (must be a power of two) and see how this affects your performance.
void cs_set_pitch(cs_playing_sound_t* sound, float pitch);

// Delays sound before actually playing it. Requires context to be passed in
// since there's a conversion from seconds to samples per second.
// If one were so inclined another version could be implemented like:
// void cs_set_delay(cs_playing_sound_t* sound, float delay, int samples_per_second)
void cs_set_delay(cs_context_t* ctx, cs_playing_sound_t* sound, float delay_in_seconds);

// Portable sleep function
void cs_sleep(int milliseconds);

// LOW-LEVEL API
cs_playing_sound_t cs_make_playing_sound(cs_loaded_sound_t* loaded);
int cs_insert_sound(cs_context_t* ctx, cs_playing_sound_t* sound); // returns 1 if sound was successfully inserted, 0 otherwise

// HIGH-LEVEL API
typedef struct cs_play_sound_def_t
{
	int paused;
	int looped;
	float volume_left;
	float volume_right;
	float pan;
	float pitch;
	float delay;
	cs_loaded_sound_t* loaded;
} cs_play_sound_def_t;

cs_playing_sound_t* cs_play_sound(cs_context_t* ctx, cs_play_sound_def_t def);

cs_play_sound_def_t cs_make_def(cs_loaded_sound_t* sound);

void cs_stop_all_sounds(cs_context_t* ctx);


static void cs_pitch_shift(float pitchShift, int num_samples_to_process, float sampleRate, float* indata, cs_pitch_data_t** pitch_filter);







inline const char* cs_error_reason;

inline static void* cs_read_file_to_memory(const char* path, int* size)
{
	void* data = 0;
	FILE* fp = fopen(path, "rb");
	int sizeNum = 0;

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		sizeNum = (int)ftell(fp);
		fseek(fp, 0, SEEK_SET);
		data = CUTE_SOUND_ALLOC(sizeNum);
		fread(data, sizeNum, 1, fp);
		fclose(fp);
	}

	if (size) *size = sizeNum;
	return data;
}

inline static int cs_four_cc(const char* CC, void* memory)
{
	if (!memcmp(CC, memory, 4)) return 1;
	return 0;
}

inline static char* cs_next(char* data)
{
	uint32_t size = *(uint32_t*)(data + 4);
	size = (size + 1) & ~1;
	return data + 8 + size;
}

inline static void* cs_malloc16(size_t size)
{
	void* p = CUTE_SOUND_ALLOC(size + 16);
	if (!p) return 0;
	unsigned char offset = (size_t)p & 15;
	p = (void*)CUTE_SOUND_ALIGN(p + 1, 16);
	*((char*)p - 1) = 16 - offset;
	CUTE_SOUND_ASSERT(!((size_t)p & 15));
	return p;
}

inline static void cs_free16(void* p)
{
	if (!p) return;
	CUTE_SOUND_FREE((char*)p - (((size_t)*((char*)p - 1)) & 0xFF));
}

inline static void cs_last_element(__m128* a, int i, int j, int16_t* samples, int offset)
{
	switch (offset)
	{
	case 1:
		a[i] = _mm_set_ps(samples[j], 0.0f, 0.0f, 0.0f);
		break;

	case 2:
		a[i] = _mm_set_ps(samples[j], samples[j + 1], 0.0f, 0.0f);
		break;

	case 3:
		a[i] = _mm_set_ps(samples[j], samples[j + 1], samples[j + 2], 0.0f);
		break;

	case 0:
		a[i] = _mm_set_ps(samples[j], samples[j + 1], samples[j + 2], samples[j + 3]);
		break;
	}
}

inline void cs_read_mem_wav(const void* memory, int size, cs_loaded_sound_t* sound)
{
	#pragma pack(push, 1)
	typedef struct
	{
		uint16_t wFormatTag;
		uint16_t nChannels;
		uint32_t nSamplesPerSec;
		uint32_t nAvgBytesPerSec;
		uint16_t nBlockAlign;
		uint16_t wBitsPerSample;
		uint16_t cbSize;
		uint16_t wValidBitsPerSample;
		uint32_t dwChannelMask;
		uint8_t SubFormat[18];
	} Fmt;
	#pragma pack(pop)

	sound->playing_count = 0;

	char* data = (char*)memory;
	char* end = data + size;
	CUTE_SOUND_CHECK(data, "Unable to read input file (file doesn't exist, or could not allocate heap memory.");
	CUTE_SOUND_CHECK(cs_four_cc("RIFF", data), "Incorrect file header; is this a WAV file?");
	CUTE_SOUND_CHECK(cs_four_cc("WAVE", data + 8), "Incorrect file header; is this a WAV file?");

	data += 12;

	while (1)
	{
		CUTE_SOUND_CHECK(end > data, "Error searching for fmt chunk.");
		if (cs_four_cc("fmt ", data)) break;
		data = cs_next(data);
	}

	Fmt fmt;
	fmt = *(Fmt*)(data + 8);
	CUTE_SOUND_CHECK(fmt.wFormatTag == 1, "Only PCM WAV files are supported.");
	CUTE_SOUND_CHECK(fmt.nChannels == 1 || fmt.nChannels == 2, "Only mono or stereo supported (too many channels detected).");
	CUTE_SOUND_CHECK(fmt.wBitsPerSample == 16, "Only 16 bits per sample supported.");
	CUTE_SOUND_CHECK(fmt.nBlockAlign == fmt.nChannels * 2, "implementation error");

	sound->sample_rate = (int)fmt.nSamplesPerSec;

	while (1)
	{
		CUTE_SOUND_CHECK(end > data, "Error searching for data chunk.");
		if (cs_four_cc("data", data)) break;
		data = cs_next(data);
	}

	{
		int sample_size = *((uint32_t*)(data + 4));
		int sample_count = sample_size / (fmt.nChannels * sizeof(uint16_t));
		sound->sample_count = sample_count;
		sound->channel_count = fmt.nChannels;

		int wide_count = (int)CUTE_SOUND_ALIGN(sample_count, 4);
		wide_count /= 4;
		int wide_offset = sample_count & 3;
		int16_t* samples = (int16_t*)(data + 8);
		float* sample = (float*)alloca(sizeof(float) * 4 + 16);
		sample = (float*)CUTE_SOUND_ALIGN(sample, 16);

		switch (sound->channel_count)
		{
		case 1:
		{
			sound->channels[0] = cs_malloc16(wide_count * sizeof(__m128));
			sound->channels[1] = 0;
			__m128* a = (__m128*)sound->channels[0];

			for (int i = 0, j = 0; i < wide_count - 1; ++i, j += 4)
			{
				sample[0] = (float)samples[j];
				sample[1] = (float)samples[j + 1];
				sample[2] = (float)samples[j + 2];
				sample[3] = (float)samples[j + 3];
				a[i] = _mm_load_ps(sample);
			}

			cs_last_element(a, wide_count - 1, (wide_count - 1) * 4, samples, wide_offset);
		}	break;

		case 2:
		{
			__m128* a = (__m128*)cs_malloc16(wide_count * sizeof(__m128) * 2);
			__m128* b = a + wide_count;

			for (int i = 0, j = 0; i < wide_count - 1; ++i, j += 8)
			{
				sample[0] = (float)samples[j];
				sample[1] = (float)samples[j + 2];
				sample[2] = (float)samples[j + 4];
				sample[3] = (float)samples[j + 6];
				a[i] = _mm_load_ps(sample);

				sample[0] = (float)samples[j + 1];
				sample[1] = (float)samples[j + 3];
				sample[2] = (float)samples[j + 5];
				sample[3] = (float)samples[j + 7];
				b[i] = _mm_load_ps(sample);
			}

			cs_last_element(a, wide_count - 1, (wide_count - 1) * 4, samples, wide_offset);
			cs_last_element(b, wide_count - 1, (wide_count - 1) * 4 + 4, samples, wide_offset);
			sound->channels[0] = a;
			sound->channels[1] = b;
		}	break;

		default:
			CUTE_SOUND_CHECK(0, "unsupported channel count (only support mono and stereo).");
		}
	}

	return;

ts_err:
	memset(&sound, 0, sizeof(sound));
}

inline cs_loaded_sound_t cs_load_wav(const char* path)
{
	cs_loaded_sound_t sound = { 0 };
	int size;
	char* wav = (char*)cs_read_file_to_memory(path, &size);
	cs_read_mem_wav(wav, size, &sound);
	CUTE_SOUND_FREE(wav);
	return sound;
}

inline void cs_free_sound(cs_loaded_sound_t* sound)
{
	cs_free16(sound->channels[0]);
	memset(sound, 0, sizeof(cs_loaded_sound_t));
}

inline int cs_sound_size(cs_loaded_sound_t* sound)
{
	return sound->sample_count * sound->channel_count * sizeof(uint16_t);
}

inline cs_playing_sound_t cs_make_playing_sound(cs_loaded_sound_t* loaded)
{
	cs_playing_sound_t playing;
	playing.active = 0;
	playing.paused = 0;
	playing.looped = 0;
	playing.volume0 = 1.0f;
	playing.volume1 = 1.0f;
	playing.pan0 = 0.5f;
	playing.pan1 = 0.5f;
	playing.pitch = 1.0f;
	playing.pitch_filter[0] = 0;
	playing.pitch_filter[1] = 0;
	playing.sample_index = 0;
	playing.loaded_sound = loaded;
	playing.next = 0;
	return playing;
}

inline int cs_is_active(cs_playing_sound_t* sound)
{
	return sound->active;
}

inline void cs_stop_sound(cs_playing_sound_t* sound)
{
	sound->loaded_sound = 0;
	sound->active = 0;
}

inline void cs_loop_sound(cs_playing_sound_t* sound, int zero_for_no_loop)
{
	sound->looped = zero_for_no_loop;
}

inline void cs_pause_sound(cs_playing_sound_t* sound, int one_for_paused)
{
	sound->paused = one_for_paused;
}

inline void cs_set_pan(cs_playing_sound_t* sound, float pan)
{
	if (pan > 1.0f) pan = 1.0f;
	else if (pan < 0.0f) pan = 0.0f;
	float left = 1.0f - pan;
	float right = pan;
	sound->pan0 = left;
	sound->pan1 = right;
}

inline void cs_set_pitch(cs_playing_sound_t* sound, float pitch)
{
	sound->pitch = pitch;
}

inline void cs_set_volume(cs_playing_sound_t* sound, float volume_left, float volume_right)
{
	if (volume_left < 0.0f) volume_left = 0.0f;
	if (volume_right < 0.0f) volume_right = 0.0f;
	sound->volume0 = volume_left;
	sound->volume1 = volume_right;
}

inline static void cs_remove_filter(cs_playing_sound_t* playing);

inline void cs_sleep(int milliseconds)
{
	Sleep(milliseconds);
}

struct cs_context_t
{
	unsigned latency_samples;
	unsigned running_index;
	int Hz;
	int bps;
	int buffer_size;
	int wide_count;
	cs_playing_sound_t* playing;
	__m128* floatA;
	__m128* floatB;
	__m128i* samples;
	cs_playing_sound_t* playing_pool;
	cs_playing_sound_t* playing_free;

	// platform specific stuff
	LPDIRECTSOUND dsound;
	LPDIRECTSOUNDBUFFER buffer;
	LPDIRECTSOUNDBUFFER primary;

	// data for cs_mix thread, enable these with cs_spawn_mix_thread
	CRITICAL_SECTION critical_section;
	int separate_thread;
	int running;
	int sleep_milliseconds;
};

inline static void cs_release_context(cs_context_t* ctx)
{
	if (ctx->separate_thread)	DeleteCriticalSection(&ctx->critical_section);

	ctx->buffer->Release();
	ctx->primary->Release();
	ctx->dsound->Release();

	cs_playing_sound_t* playing = ctx->playing;
	while (playing)
	{
		cs_remove_filter(playing);
		playing = playing->next;
	}
	CUTE_SOUND_FREE(ctx);
}

inline static DWORD WINAPI cs_ctx_thread(LPVOID lpParameter)
{
	cs_context_t* ctx = (cs_context_t*)lpParameter;

	while (ctx->running)
	{
		cs_mix(ctx);
		if (ctx->sleep_milliseconds) cs_sleep(ctx->sleep_milliseconds);
		else YieldProcessor();
	}

	ctx->separate_thread = 0;
	return 0;
}

inline static void cs_lock(cs_context_t* ctx)
{
	if (ctx->separate_thread) EnterCriticalSection(&ctx->critical_section);
}

inline static void cs_unlock(cs_context_t* ctx)
{
	if (ctx->separate_thread) LeaveCriticalSection(&ctx->critical_section);
}

inline cs_context_t* cs_make_context(void* hwnd, unsigned play_frequency_in_Hz, int latency_factor_in_Hz, int num_buffered_seconds, int playing_pool_count)
{
	int bps = sizeof(INT16) * 2;
	int buffer_size = play_frequency_in_Hz * bps * num_buffered_seconds;
	cs_context_t* ctx = 0;
	WAVEFORMATEX format = { 0 };
	DSBUFFERDESC bufdesc = { 0 };
	LPDIRECTSOUND dsound;

	CUTE_SOUND_CHECK(hwnd, "Invalid hwnd passed to cs_make_context.");
	{
		HRESULT res = DirectSoundCreate(0, &dsound, 0);
		CUTE_SOUND_CHECK(res == DS_OK, "DirectSoundCreate failed");

		dsound->SetCooperativeLevel((HWND)hwnd, DSSCL_PRIORITY);

		bufdesc.dwSize = sizeof(bufdesc);
		bufdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

		LPDIRECTSOUNDBUFFER primary_buffer;

		res = dsound->CreateSoundBuffer(&bufdesc, &primary_buffer, 0);

		CUTE_SOUND_CHECK(res == DS_OK, "Failed to create primary sound buffer");

		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = 2;
		format.nSamplesPerSec = play_frequency_in_Hz;
		format.wBitsPerSample = 16;
		format.nBlockAlign = (format.nChannels * format.wBitsPerSample) / 8;
		format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
		format.cbSize = 0;

		res = primary_buffer->SetFormat(&format);

		CUTE_SOUND_CHECK(res == DS_OK, "Failed to set format on primary buffer");

		LPDIRECTSOUNDBUFFER secondary_buffer;
		bufdesc.dwSize = sizeof(bufdesc);
		bufdesc.dwFlags = 0;
		bufdesc.dwBufferBytes = buffer_size;
		bufdesc.lpwfxFormat = &format;

		res = dsound->CreateSoundBuffer(&bufdesc, &secondary_buffer, 0);

		CUTE_SOUND_CHECK(res == DS_OK, "Failed to set format on secondary buffer");

		int sample_count = play_frequency_in_Hz * num_buffered_seconds;
		int wide_count = (int)CUTE_SOUND_ALIGN(sample_count, 4);
		int pool_size = playing_pool_count * sizeof(cs_playing_sound_t);
		int mix_buffers_size = sizeof(__m128) * wide_count * 2;
		int sample_buffer_size = sizeof(__m128i) * wide_count;
		ctx = (cs_context_t*)CUTE_SOUND_ALLOC(sizeof(cs_context_t) + mix_buffers_size + sample_buffer_size + 16 + pool_size);
		ctx->latency_samples = (unsigned)CUTE_SOUND_ALIGN(play_frequency_in_Hz / latency_factor_in_Hz, 4);
		ctx->running_index = 0;
		ctx->Hz = play_frequency_in_Hz;
		ctx->bps = bps;
		ctx->buffer_size = buffer_size;
		ctx->wide_count = wide_count;
		ctx->dsound = dsound;
		ctx->buffer = secondary_buffer;
		ctx->primary = primary_buffer;
		ctx->playing = 0;
		ctx->floatA = (__m128*)(ctx + 1);
		ctx->floatA = (__m128*)CUTE_SOUND_ALIGN(ctx->floatA, 16);
		CUTE_SOUND_ASSERT(!((size_t)ctx->floatA & 15));
		ctx->floatB = ctx->floatA + wide_count;
		ctx->samples = (__m128i*)ctx->floatB + wide_count;
		ctx->running = 1;
		ctx->separate_thread = 0;
		ctx->sleep_milliseconds = 0;

		if (playing_pool_count)
		{
			ctx->playing_pool = (cs_playing_sound_t*)(ctx->samples + wide_count);
			for (int i = 0; i < playing_pool_count - 1; ++i)
				ctx->playing_pool[i].next = ctx->playing_pool + i + 1;
			ctx->playing_pool[playing_pool_count - 1].next = 0;
			ctx->playing_free = ctx->playing_pool;
		}

		else
		{
			ctx->playing_pool = 0;
			ctx->playing_free = 0;
		}

		return ctx;
	}

ts_err:
	CUTE_SOUND_FREE(ctx);
	return 0;
}

inline void cs_spawn_mix_thread(cs_context_t* ctx)
{
	if (ctx->separate_thread) return;
	InitializeCriticalSectionAndSpinCount(&ctx->critical_section, 0x00000400);
	ctx->separate_thread = 1;
	CreateThread(0, 0, cs_ctx_thread, ctx, 0, 0);
}

inline void cs_shutdown_context(cs_context_t* ctx)
{
	if (ctx->separate_thread)
	{
		cs_lock(ctx);
		ctx->running = 0;
		cs_unlock(ctx);
	}

	while (ctx->separate_thread) cs_sleep(1);
	cs_release_context(ctx);
}

inline void cs_thread_sleep_delay(cs_context_t* ctx, int milliseconds)
{
	ctx->sleep_milliseconds = milliseconds;
}

inline int cs_insert_sound(cs_context_t* ctx, cs_playing_sound_t* sound)
{
	// Cannot use cs_playing_sound_t if cs_make_context was passed non-zero for playing_pool_count
	// since non-zero playing_pool_count means the context is doing some memory-management
	// for a playing sound pool. InsertSound assumes the pool does not exist, and is apart
	// of the lower-level API (see top of this header for documentation details).
	CUTE_SOUND_ASSERT(ctx->playing_pool == 0);

	if (sound->active) return 0;
	cs_lock(ctx);
	sound->next = ctx->playing;
	ctx->playing = sound;
	sound->loaded_sound->playing_count += 1;
	sound->active = 1;
	cs_unlock(ctx);
	return 1;
}

inline void cs_set_delay(cs_context_t* ctx, cs_playing_sound_t* sound, float delay_in_seconds)
{
	if (delay_in_seconds < 0.0f) delay_in_seconds = 0.0f;
	sound->sample_index = (int)(delay_in_seconds * (float)ctx->Hz);
	sound->sample_index = -(int)CUTE_SOUND_ALIGN(sound->sample_index, 4);
}

inline cs_play_sound_def_t cs_make_def(cs_loaded_sound_t* sound)
{
	cs_play_sound_def_t def;
	def.paused = 0;
	def.looped = 0;
	def.volume_left = 1.0f;
	def.volume_right = 1.0f;
	def.pan = 0.5f;
	def.pitch = 1.0f;
	def.delay = 0.0f;
	def.loaded = sound;
	return def;
}

inline cs_playing_sound_t* cs_play_sound(cs_context_t* ctx, cs_play_sound_def_t def)
{
	cs_lock(ctx);

	cs_playing_sound_t* playing = ctx->playing_free;
	if (!playing) return 0;
	ctx->playing_free = playing->next;
	*playing = cs_make_playing_sound(def.loaded);
	playing->active = 1;
	playing->paused = def.paused;
	playing->looped = def.looped;
	cs_set_volume(playing, def.volume_left, def.volume_right);
	cs_set_pan(playing, def.pan);
	cs_set_pitch(playing, def.pitch);
	cs_set_delay(ctx, playing, def.delay);
	playing->next = ctx->playing;
	ctx->playing = playing;
	playing->loaded_sound->playing_count += 1;

	cs_unlock(ctx);

	return playing;
}

inline void cs_stop_all_sounds(cs_context_t* ctx)
{
	// This is apart of the high level API, not the low level API.
	// If using the low level API you must write your own function to
	// stop playing all sounds.
	CUTE_SOUND_ASSERT(ctx->playing_pool != 0);

	cs_lock(ctx);
	cs_playing_sound_t* sound = ctx->playing;
	while (sound)
	{
		// let cs_mix() remove the sound
		sound->active = 0;
		sound = sound->next;
	}
	cs_unlock(ctx);
}

inline static void cs_position(cs_context_t* ctx, int* byte_to_lock, int* bytes_to_write)
{
	// compute bytes to be written to direct sound
	DWORD play_cursor;
	DWORD write_cursor;

	HRESULT hr = ctx->buffer->GetCurrentPosition(&play_cursor, &write_cursor);

	CUTE_SOUND_ASSERT(hr == DS_OK);

	DWORD lock = (ctx->running_index * ctx->bps) % ctx->buffer_size;
	DWORD target_cursor = (write_cursor + ctx->latency_samples * ctx->bps) % ctx->buffer_size;
	target_cursor = (DWORD)CUTE_SOUND_ALIGN(target_cursor, 16);
	DWORD write;

	if (lock > target_cursor)
	{
		write = (ctx->buffer_size - lock) + target_cursor;
	}

	else
	{
		write = target_cursor - lock;
	}

	*byte_to_lock = lock;
	*bytes_to_write = write;
}

inline static void cs_memcpy_to_directsound(cs_context_t* ctx, int16_t* samples, int byte_to_lock, int bytes_to_write)
{
	// copy mixer buffers to direct sound
	void* region1;
	DWORD size1;
	void* region2;
	DWORD size2;

	HRESULT hr = ctx->buffer->Lock(byte_to_lock, bytes_to_write, &region1, &size1, &region2, &size2, 0);

	if (hr == DSERR_BUFFERLOST)
	{
		ctx->buffer->Restore();
		hr = ctx->buffer->Lock(byte_to_lock, bytes_to_write, &region1, &size1, &region2, &size2, 0);
	}

	if (!SUCCEEDED(hr))
		return;

	unsigned running_index = ctx->running_index;
	INT16* sample1 = (INT16*)region1;
	DWORD sample1_count = size1 / ctx->bps;
	memcpy(sample1, samples, sample1_count * sizeof(INT16) * 2);
	samples += sample1_count * 2;
	running_index += sample1_count;

	INT16* sample2 = (INT16*)region2;
	DWORD sample2_count = size2 / ctx->bps;
	memcpy(sample2, samples, sample2_count * sizeof(INT16) * 2);
	samples += sample2_count * 2;
	running_index += sample2_count;

	ctx->buffer->Unlock(region1, size1, region2, size2);
	ctx->running_index = running_index;

	// meager hack to fill out sound buffer before playing
	static int first;
	if (!first)
	{
		ctx->buffer->Play(0, 0, DSBPLAY_LOOPING);
		first = 1;
	}
}

typedef struct cs_pitch_data_t
{
	float pitch_shifted_output_samples[CUTE_SOUND_MAX_FRAME_LENGTH];
	float in_FIFO[CUTE_SOUND_STEPSIZE + CUTE_SOUND_PITCH_FRAME_SIZE];
	float out_FIFO[CUTE_SOUND_STEPSIZE + CUTE_SOUND_PITCH_FRAME_SIZE];
	float fft_data[2 * CUTE_SOUND_PITCH_FRAME_SIZE];
	float previous_phase[CUTE_SOUND_PITCH_FRAME_SIZE / 2 + 4];
	float sum_phase[CUTE_SOUND_PITCH_FRAME_SIZE / 2 + 4];
	float window_accumulator[CUTE_SOUND_STEPSIZE + CUTE_SOUND_PITCH_FRAME_SIZE];
	float freq[CUTE_SOUND_PITCH_FRAME_SIZE];
	float mag[CUTE_SOUND_PITCH_FRAME_SIZE];
	float pitch_shift_workspace[CUTE_SOUND_PITCH_FRAME_SIZE];
	int index;
} cs_pitch_data_t;

inline static void cs_remove_filter(cs_playing_sound_t* playing)
{
	for (int i = 0; i < 2; i++)
	{
		if (playing->pitch_filter[i])
		{
			cs_free16(playing->pitch_filter[i]);
			playing->pitch_filter[i] = 0;
		}
	}
}

inline void cs_mix(cs_context_t* ctx)
{
	cs_lock(ctx);

	int byte_to_lock;
	int bytes_to_write;
	cs_position(ctx, &byte_to_lock, &bytes_to_write);

	int samples_to_write;
	int wide_count;
	__m128* floatA;
	__m128* floatB;
	__m128 zero;
	cs_playing_sound_t** ptr;
	__m128i* samples;

	if (!bytes_to_write) goto unlock;
	samples_to_write = bytes_to_write / ctx->bps;

	// clear mixer buffers
	wide_count = samples_to_write / 4;
	CUTE_SOUND_ASSERT(!(samples_to_write & 3));

	floatA = ctx->floatA;
	floatB = ctx->floatB;
	zero = _mm_set1_ps(0.0f);

	for (int i = 0; i < wide_count; ++i)
	{
		floatA[i] = zero;
		floatB[i] = zero;
	}

	// mix all playing sounds into the mixer buffers
	ptr = &ctx->playing;
	while (*ptr)
	{
		cs_playing_sound_t* playing = *ptr;
		cs_loaded_sound_t* loaded = playing->loaded_sound;

		// immediately remove any inactive elements
		if (!playing->active || !ctx->running)
			goto remove;

		if (!loaded)
			goto remove;

		// skip all paused sounds
		if (playing->paused)
			goto get_next_playing_sound;

		{
			__m128* cA = (__m128*)loaded->channels[0];
			__m128* cB = (__m128*)loaded->channels[1];

			// Attempted to play a sound with no audio.
			// Make sure the audio file was loaded properly. Check for
			// error messages in cs_error_reason.
			CUTE_SOUND_ASSERT(cA);

			int mix_count = samples_to_write;
			int offset = playing->sample_index;
			int remaining = loaded->sample_count - offset;
			if (remaining < mix_count) mix_count = remaining;
			CUTE_SOUND_ASSERT(remaining > 0);

			float vA0 = playing->volume0 * playing->pan0;
			float vB0 = playing->volume1 * playing->pan1;
			__m128 vA = _mm_set1_ps(vA0);
			__m128 vB = _mm_set1_ps(vB0);

			// skip sound if it's delay is longer than mix_count and
			// handle various delay cases
			int delay_offset = 0;
			if (offset < 0)
			{
				int samples_till_positive = -offset;
				int mix_leftover = mix_count - samples_till_positive;

				if (mix_leftover <= 0)
				{
					playing->sample_index += mix_count;
					goto get_next_playing_sound;
				}

				else
				{
					offset = 0;
					delay_offset = samples_till_positive;
					mix_count = mix_leftover;
				}
			}
			CUTE_SOUND_ASSERT(!(delay_offset & 3));

			// SIMD offets
			int mix_wide = (int)CUTE_SOUND_ALIGN(mix_count, 4) / 4;
			int offset_wide = (int)CUTE_SOUND_TRUNC(offset, 4) / 4;
			int delay_wide = (int)CUTE_SOUND_ALIGN(delay_offset, 4) / 4;

			// use cs_pitch_shift to on-the-fly pitch shift some samples
			// only call this function if the user set a custom pitch value
			float pitch = playing->pitch;
			if (pitch != 1.0f)
			{
				int sample_count = (mix_wide - 2 * delay_wide) * 4;
				int falling_behind = sample_count > CUTE_SOUND_MAX_FRAME_LENGTH;

				// CUTE_SOUND_MAX_FRAME_LENGTH represents max samples we can pitch shift in one go. In the event
				// that this process takes longer than the time required to play the actual sound, just
				// fall back to the original sound (non-pitch shifted). This will sound very ugly. To
				// prevent falling behind, make sure not to pitch shift too many sounds at once. Try tweaking
				// CUTE_SOUND_PITCH_QUALITY to make it lower (must be a power of 2).
				if (!falling_behind)
				{
					cs_pitch_shift(pitch, sample_count, (float)ctx->Hz, (float*)(cA + delay_wide + offset_wide), playing->pitch_filter);
					cA = (__m128 *)playing->pitch_filter[0]->pitch_shifted_output_samples;

					if (loaded->channel_count == 2)
					{
						cs_pitch_shift(pitch, sample_count, (float)ctx->Hz, (float*)(cB + delay_wide + offset_wide), playing->pitch_filter + 1);
						cB = (__m128 *)playing->pitch_filter[1]->pitch_shifted_output_samples;
					}

					offset_wide = -delay_wide;
				}
			}

			// apply volume, load samples into float buffers
			switch (loaded->channel_count)
			{
			case 1:
				for (int i = delay_wide; i < mix_wide - delay_wide; ++i)
				{
					__m128 A = cA[i + offset_wide];
					__m128 B = _mm_mul_ps(A, vB);
					A = _mm_mul_ps(A, vA);
					floatA[i] = _mm_add_ps(floatA[i], A);
					floatB[i] = _mm_add_ps(floatB[i], B);
				}
				break;

			case 2:
			{
				for (int i = delay_wide; i < mix_wide - delay_wide; ++i)
				{
					__m128 A = cA[i + offset_wide];
					__m128 B = cB[i + offset_wide];

					A = _mm_mul_ps(A, vA);
					B = _mm_mul_ps(B, vB);
					floatA[i] = _mm_add_ps(floatA[i], A);
					floatB[i] = _mm_add_ps(floatB[i], B);
				}
			}	break;
			}

			// playing list logic
			playing->sample_index += mix_count;
			if (playing->sample_index == loaded->sample_count)
			{
				if (playing->looped)
				{
					playing->sample_index = 0;
					goto get_next_playing_sound;
				}

				goto remove;
			}
		}

	get_next_playing_sound:
		if (*ptr) ptr = &(*ptr)->next;
		else break;
		continue;

	remove:
		playing->sample_index = 0;
		*ptr = (*ptr)->next;
		playing->next = 0;
		playing->active = 0;

		if (playing->loaded_sound)
		{
			playing->loaded_sound->playing_count -= 1;
			CUTE_SOUND_ASSERT(playing->loaded_sound->playing_count >= 0);
		}

		cs_remove_filter(playing);

		// if using high-level API manage the cs_playing_sound_t memory ourselves
		if (ctx->playing_pool)
		{
			playing->next = ctx->playing_free;
			ctx->playing_free = playing;
		}

		continue;
	}

	// load all floats into 16 bit packed interleaved samples
	samples = ctx->samples;
	for (int i = 0; i < wide_count; ++i)
	{
		__m128i a = _mm_cvtps_epi32(floatA[i]);
		__m128i b = _mm_cvtps_epi32(floatB[i]);
		__m128i a0b0a1b1 = _mm_unpacklo_epi32(a, b);
		__m128i a2b2a3b3 = _mm_unpackhi_epi32(a, b);
		samples[i] = _mm_packs_epi32(a0b0a1b1, a2b2a3b3);
	}
	cs_memcpy_to_directsound(ctx, (int16_t*)samples, byte_to_lock, bytes_to_write);

	unlock:
	cs_unlock(ctx);
}

inline static uint32_t cs_rev32(uint32_t x)
{
	uint32_t a = ((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1);
	uint32_t b = ((a & 0xCCCCCCCC) >> 2) | ((a & 0x33333333) << 2);
	uint32_t c = ((b & 0xF0F0F0F0) >> 4) | ((b & 0x0F0F0F0F) << 4);
	uint32_t d = ((c & 0xFF00FF00) >> 8) | ((c & 0x00FF00FF) << 8);
	return (d >> 16) | (d << 16);
}

inline static uint32_t cx_popcount(uint32_t x)
{
	uint32_t a = x - ((x >> 1) & 0x55555555);
	uint32_t b = (((a >> 2) & 0x33333333) + (a & 0x33333333));
	uint32_t c = (((b >> 4) + b) & 0x0F0F0F0F);
	uint32_t d = c + (c >> 8);
	uint32_t e = d + (d >> 16);
	uint32_t f = e & 0x0000003F;
	return f;
}

inline static uint32_t cs_log2(uint32_t x)
{
	uint32_t a = x | (x >> 1);
	uint32_t b = a | (a >> 2);
	uint32_t c = b | (b >> 4);
	uint32_t d = c | (c >> 8);
	uint32_t e = d | (d >> 16);
	uint32_t f = e >> 1;
	return cx_popcount(f);
}

inline static void cs_fft(float* x, float* y, int count, float sign)
{
	int exponent = (int)cs_log2((uint32_t)count);

	// bit reversal stage
	// swap all elements with their bit reversed index within the
	// lowest level of the Cooley-Tukey recursion tree
	for (int i = 1; i < count - 1; i++)
	{
		uint32_t j = cs_rev32((uint32_t)i);
		j >>= (32 - exponent);
		if (i < (int)j)
		{
			float tx = x[i];
			float ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
		}
	}

	// for each recursive iteration
	for (int iter = 0, L = 1; iter < exponent; ++iter)
	{
		int Ls = L;
		L <<= 1;
		float ur = 1.0f; // cos(pi / 2)
		float ui = 0;    // sin(pi / 2)
		float arg = 3.14159265359f / (float)Ls;
		float wr = cosf(arg);
		float wi = -sign * sinf(arg);

		// rows in DFT submatrix
		for (int j = 0; j < Ls; ++j)
		{
			// do butterflies upon DFT row elements
			for (int i = j; i < count; i += L)
			{
				int index = i + Ls;
				float x_index = x[index];
				float y_index = y[index];
				float x_i = x[i];
				float y_i = y[i];

				float tr = ur * x_index - ui * y_index;
				float ti = ur * y_index + ui * x_index;
				float x_low = x_i - tr;
				float x_high = x_i + tr;
				float y_low = y_i - ti;
				float y_high = y_i + ti;

				x[index] = x_low;
				y[index] = y_low;
				x[i] = x_high;
				y[i] = y_high;
			}

			// Rotate u1 and u2 via Givens rotations (2d planar rotation).
			// This keeps cos/sin calls in the outermost loop.
			// Floating point error is scaled proportionally to Ls.
			float t = ur * wr - ui * wi;
			ui = ur * wi + ui * wr;
			ur = t;
		}
	}

	// scale factor for forward transform
	if (sign > 0)
	{
		float inv_count = 1.0f / (float)count;
		for (int i = 0; i < count; i++)
		{
			x[i] *= inv_count;
			y[i] *= inv_count;
		}
	}
}

inline static void _mm_sincos_ps(__m128 x, __m128 *s, __m128 *c)
{
	__m128 xmm1, xmm2, xmm3 = _mm_setzero_ps(), sign_bit_sin, y;
	__m128i emm0, emm2, emm4;
	sign_bit_sin = x;
	/* take the absolute value */
	x = _mm_and_ps(x, *(__m128*)_ps_inv_sign_mask);
	/* extract the sign bit (upper one) */
	sign_bit_sin = _mm_and_ps(sign_bit_sin, *(__m128*)_ps_sign_mask);

	/* scale by 4/Pi */
	y = _mm_mul_ps(x, *(__m128*)_ps_cephes_FOPI);

	/* store the integer part of y in emm2 */
	emm2 = _mm_cvttps_epi32(y);

	/* j=(j+1) & (~1) (see the cephes sources) */
	emm2 = _mm_add_epi32(emm2, *(__m128i*)_pi32_1);
	emm2 = _mm_and_si128(emm2, *(__m128i*)_pi32_inv1);
	y = _mm_cvtepi32_ps(emm2);

	emm4 = emm2;

	/* get the swap sign flag for the sine */
	emm0 = _mm_and_si128(emm2, *(__m128i*)_pi32_4);
	emm0 = _mm_slli_epi32(emm0, 29);
	__m128 swap_sign_bit_sin = _mm_castsi128_ps(emm0);

	/* get the polynom selection mask for the sine*/
	emm2 = _mm_and_si128(emm2, *(__m128i*)_pi32_2);
	emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
	__m128 poly_mask = _mm_castsi128_ps(emm2);

	/* The magic pass: "Extended precision modular arithmetic"
		x = ((x - y * DP1) - y * DP2) - y * DP3; */
	xmm1 = *(__m128*)_ps_minus_cephes_DP1;
	xmm2 = *(__m128*)_ps_minus_cephes_DP2;
	xmm3 = *(__m128*)_ps_minus_cephes_DP3;
	xmm1 = _mm_mul_ps(y, xmm1);
	xmm2 = _mm_mul_ps(y, xmm2);
	xmm3 = _mm_mul_ps(y, xmm3);
	x = _mm_add_ps(x, xmm1);
	x = _mm_add_ps(x, xmm2);
	x = _mm_add_ps(x, xmm3);

	emm4 = _mm_sub_epi32(emm4, *(__m128i*)_pi32_2);
	emm4 = _mm_andnot_si128(emm4, *(__m128i*)_pi32_4);
	emm4 = _mm_slli_epi32(emm4, 29);
	__m128 sign_bit_cos = _mm_castsi128_ps(emm4);

	sign_bit_sin = _mm_xor_ps(sign_bit_sin, swap_sign_bit_sin);


	/* Evaluate the first polynom  (0 <= x <= Pi/4) */
	__m128 z = _mm_mul_ps(x, x);
	y = *(__m128*)_ps_coscof_p0;

	y = _mm_mul_ps(y, z);
	y = _mm_add_ps(y, *(__m128*)_ps_coscof_p1);
	y = _mm_mul_ps(y, z);
	y = _mm_add_ps(y, *(__m128*)_ps_coscof_p2);
	y = _mm_mul_ps(y, z);
	y = _mm_mul_ps(y, z);
	__m128 tmp = _mm_mul_ps(z, *(__m128*)_ps_0p5);
	y = _mm_sub_ps(y, tmp);
	y = _mm_add_ps(y, *(__m128*)_ps_1);

	/* Evaluate the second polynom  (Pi/4 <= x <= 0) */

	__m128 y2 = *(__m128*)_ps_sincof_p0;
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_add_ps(y2, *(__m128*)_ps_sincof_p1);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_add_ps(y2, *(__m128*)_ps_sincof_p2);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_mul_ps(y2, x);
	y2 = _mm_add_ps(y2, x);

	/* select the correct result from the two polynoms */
	xmm3 = poly_mask;
	__m128 ysin2 = _mm_and_ps(xmm3, y2);
	__m128 ysin1 = _mm_andnot_ps(xmm3, y);
	y2 = _mm_sub_ps(y2, ysin2);
	y = _mm_sub_ps(y, ysin1);

	xmm1 = _mm_add_ps(ysin1, ysin2);
	xmm2 = _mm_add_ps(y, y2);

	/* update the sign */
	*s = _mm_xor_ps(xmm1, sign_bit_sin);
	*c = _mm_xor_ps(xmm2, sign_bit_cos);
}

inline static __m128i select_si(__m128i a, __m128i b, __m128i mask)
{
	return _mm_xor_si128(a, _mm_and_si128(mask, _mm_xor_si128(b, a)));
}

inline static __m128 cs_vonhann4(int i)
{
	__m128 k4 = _mm_set_ps((float)(i * 4 + 3), (float)(i * 4 + 2), (float)(i * 4 + 1), (float)(i * 4));
	k4 = _mm_mul_ps(*(__m128*)_ps_cephes_2PIF, k4);
	k4 = _mm_div_ps(k4, *(__m128*)_ps_frame_size);

	// Seems like _mm_cos_ps and _mm_sincos_ps was causing some audio popping...
	// I'm not really skilled enough to fix it, but feel free to try: http://gruntthepeon.free.fr/ssemath/sse_mathfun.h
	// My guess is some large negative or positive values were causing some
	// precision trouble. In this case manually calling 4 cosines is not
	// really a big deal, since this function is not a bottleneck.

	__m128 c = k4;
	float* cf = (float*)&c;
	cf[0] = cosf(cf[0]);
	cf[1] = cosf(cf[1]);
	cf[2] = cosf(cf[2]);
	cf[3] = cosf(cf[3]);

	__m128 von_hann = _mm_add_ps(_mm_mul_ps(_mm_set_ps1(-0.5f), c), _mm_set_ps1(0.5f));
	return von_hann;
}

inline float smbAtan2f(float x, float y)
{
	float signx = x > 0 ? 1.0f : -1.0f;
	if (x == 0) return 0;
	if (y == 0) return signx * 3.14159265f / 2.0f;
	return atan2f(x, y);
}

inline static void cs_pitch_shift(float pitchShift, int num_samples_to_process, float sampleRate, float* indata, cs_pitch_data_t** pitch_filter)
{
	CUTE_SOUND_ASSERT(num_samples_to_process <= CUTE_SOUND_MAX_FRAME_LENGTH);

	// make sure compiler didn't do anything weird with the member
	// offsets of cs_pitch_data_t. All arrays must be 16 byte aligned
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->pitch_shifted_output_samples) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->fft_data) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->previous_phase) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->sum_phase) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->window_accumulator) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->freq) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->mag) & 15));
	CUTE_SOUND_ASSERT(!((size_t)&(((cs_pitch_data_t*)0)->pitch_shift_workspace) & 15));

	cs_pitch_data_t* pf;

	if (*pitch_filter == NULL)
	{
		pf = (cs_pitch_data_t*)cs_malloc16(sizeof(cs_pitch_data_t));
		memset(pf, 0, sizeof(cs_pitch_data_t));
		*pitch_filter = pf;
	}
	else
	{
		pf = *pitch_filter;
	}

	float freqPerBin = sampleRate / (float)CUTE_SOUND_PITCH_FRAME_SIZE;
	__m128 freq_per_bin = _mm_set_ps1(sampleRate / (float)CUTE_SOUND_PITCH_FRAME_SIZE);
	__m128 pi = *(__m128*)_ps_cephes_PIF;
	__m128 two_pi = *(__m128*)_ps_cephes_2PIF;
	__m128 pitch_quality = _mm_set_ps1((float)CUTE_SOUND_PITCH_QUALITY);
	float* out_samples = pf->pitch_shifted_output_samples;
	if (pf->index == 0) pf->index = CUTE_SOUND_OVERLAP;

	while (num_samples_to_process)
	{
		int copy_count = CUTE_SOUND_PITCH_FRAME_SIZE - pf->index;
		if (num_samples_to_process < copy_count) copy_count = num_samples_to_process;

		memcpy(pf->in_FIFO + pf->index, indata, sizeof(float) * copy_count);
		memcpy(out_samples, pf->out_FIFO + pf->index - CUTE_SOUND_OVERLAP, sizeof(float) * copy_count);

		int start_index = pf->index;
		int offset = start_index & 3;
		start_index += 4 - offset;

		for (int i = 0; i < offset; ++i)
			pf->in_FIFO[pf->index + i] /= 32768.0f;

		int extra = copy_count & 3;
		copy_count = copy_count / 4 - extra;
		__m128* in_FIFO = (__m128*)(pf->in_FIFO + pf->index + offset);
		CUTE_SOUND_ASSERT(!((size_t)in_FIFO & 15));
		__m128 int16_max = _mm_set_ps1(32768.0f);

		for (int i = 0; i < copy_count; ++i)
		{
			__m128 val = in_FIFO[i];
			__m128 div = _mm_div_ps(val, int16_max);
			in_FIFO[i] = div;
		}

		for (int i = 0, copy_count4 = copy_count * 4; i < extra; ++i)
		{
			int index = copy_count4 + i;
			pf->in_FIFO[pf->index + index] /= 32768.0f;
		}

		CUTE_SOUND_ASSERT(!((size_t)out_samples & 15));
		__m128* out_samples4 = (__m128*)out_samples;
		for (int i = 0; i < copy_count; ++i)
		{
			__m128 val = out_samples4[i];
			__m128 mul = _mm_mul_ps(val, int16_max);
			out_samples4[i] = mul;
		}

		for (int i = 0, copy_count4 = copy_count * 4; i < extra; ++i)
		{
			int index = copy_count4 + i;
			out_samples[index] *= 32768.0f;
		}

		copy_count = copy_count * 4 + extra;
		num_samples_to_process -= copy_count;
		pf->index += copy_count;
		indata += copy_count;
		out_samples += copy_count;

		if (pf->index >= CUTE_SOUND_PITCH_FRAME_SIZE)
		{
			pf->index = CUTE_SOUND_OVERLAP;
			{
				__m128* fft_data = (__m128*)pf->fft_data;
				__m128* in_FIFO = (__m128*)pf->in_FIFO;

				for (int k = 0; k < CUTE_SOUND_PITCH_FRAME_SIZE / 4; k++)
				{
					__m128 von_hann = cs_vonhann4(k);
					__m128 sample = in_FIFO[k];
					__m128 windowed_sample = _mm_mul_ps(sample, von_hann);
					fft_data[k] = windowed_sample;
				}
			}

			memset(pf->fft_data + CUTE_SOUND_PITCH_FRAME_SIZE, 0, CUTE_SOUND_PITCH_FRAME_SIZE * sizeof(float));
			cs_fft(pf->fft_data, pf->fft_data + CUTE_SOUND_PITCH_FRAME_SIZE, CUTE_SOUND_PITCH_FRAME_SIZE, 1.0f);

			{
				__m128* fft_data = (__m128*)pf->fft_data;
				__m128* previous_phase = (__m128*)pf->previous_phase;
				__m128* magnitudes = (__m128*)pf->mag;
				__m128* frequencies = (__m128*)pf->freq;
				int simd_count = (CUTE_SOUND_PITCH_FRAME_SIZE / 2) / 4;

				for (int k = 0; k <= simd_count; k++)
				{
					__m128 real = fft_data[k];
					__m128 imag = fft_data[(CUTE_SOUND_PITCH_FRAME_SIZE / 4) + k];
					__m128 overlap_phase = _mm_set_ps((float)(k * 4 + 3) * CUTE_SOUND_EXPECTED_FREQUENCY, (float)(k * 4 + 2) * CUTE_SOUND_EXPECTED_FREQUENCY, (float)(k * 4 + 1) * CUTE_SOUND_EXPECTED_FREQUENCY, (float)(k * 4) * CUTE_SOUND_EXPECTED_FREQUENCY);
					__m128 k4 = _mm_set_ps((float)(k * 4 + 3), (float)(k * 4 + 2), (float)(k * 4 + 1), (float)(k * 4));
					__m128 mag = _mm_mul_ps(_mm_set_ps1(2.0f), _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(real, real), _mm_mul_ps(imag, imag))));
					__m128 phase; // = _mm_atan2_ps(imag, real);
					float *phasef = (float*)&phase;
					float *realf = (float*)&real;
					float *imagf = (float*)&imag;
					for (int i = 0; i < 4; i++) phasef[i] = smbAtan2f(imagf[i], realf[i]);
					__m128 phase_dif = _mm_sub_ps(phase, previous_phase[k]);

					previous_phase[k] = phase;
					phase_dif = _mm_sub_ps(phase_dif, overlap_phase);

					// map delta phase into +/- pi interval
					__m128i qpd = _mm_cvttps_epi32(_mm_div_ps(phase_dif, pi));
					__m128i zero = _mm_setzero_si128();
					__m128i ltzero_mask = _mm_cmplt_epi32(qpd, zero);
					__m128i ones_bit = _mm_and_si128(qpd, _mm_set1_epi32(1));
					__m128i neg_qpd = _mm_sub_epi32(qpd, ones_bit);
					__m128i pos_qpd = _mm_add_epi32(qpd, ones_bit);
					qpd = select_si(pos_qpd, neg_qpd, ltzero_mask);
					__m128 pi_range_offset = _mm_mul_ps(pi, _mm_cvtepi32_ps(qpd));
					phase_dif = _mm_sub_ps(phase_dif, pi_range_offset);

					__m128 deviation = _mm_div_ps(_mm_mul_ps(_mm_set_ps1((float)CUTE_SOUND_PITCH_QUALITY), phase_dif), two_pi);
					__m128 true_freq_estimated = _mm_add_ps(_mm_mul_ps(k4, freq_per_bin), _mm_mul_ps(deviation, freq_per_bin));

					magnitudes[k] = mag;
					frequencies[k] = true_freq_estimated;
				}
			}

			// actual pitch shifting work
			// shift frequencies into workspace
			memset(pf->pitch_shift_workspace, 0, (CUTE_SOUND_PITCH_FRAME_SIZE / 2) * sizeof(float));
			for (int k = 0; k <= CUTE_SOUND_PITCH_FRAME_SIZE / 2; k++)
			{
				int index = (int)(k * pitchShift);
				if (index <= CUTE_SOUND_PITCH_FRAME_SIZE / 2)
					pf->pitch_shift_workspace[index] = pf->freq[k] * pitchShift;
			}

			// swap buffers around to reuse old pf->preq buffer as the new workspace
			float* frequencies = pf->pitch_shift_workspace;
			float* pitch_shift_workspace = pf->freq;
			float* magnitudes = pf->mag;

			// shift magnitudes into workspace
			memset(pitch_shift_workspace, 0, CUTE_SOUND_PITCH_FRAME_SIZE * sizeof(float));
			for (int k = 0; k <= CUTE_SOUND_PITCH_FRAME_SIZE / 2; k++)
			{
				int index = (int)(k * pitchShift);
				if (index <= CUTE_SOUND_PITCH_FRAME_SIZE / 2)
					pitch_shift_workspace[index] += magnitudes[k];
			}

			// track where the shifted magnitudes are
			magnitudes = pitch_shift_workspace;

			{
				__m128* magnitudes4 = (__m128*)magnitudes;
				__m128* frequencies4 = (__m128*)frequencies;
				__m128* fft_data = (__m128*)pf->fft_data;
				__m128* sum_phase = (__m128*)pf->sum_phase;
				int simd_count = (CUTE_SOUND_PITCH_FRAME_SIZE / 2) / 4;

				for (int k = 0; k <= simd_count; k++)
				{
					__m128 mag = magnitudes4[k];
					__m128 freq = frequencies4[k];
					__m128 freq_per_bin_k = _mm_set_ps((float)(k * 4 + 3) * freqPerBin, (float)(k * 4 + 2) * freqPerBin, (float)(k * 4 + 1) * freqPerBin, (float)(k * 4) * freqPerBin);

					freq = _mm_sub_ps(freq, freq_per_bin_k);
					freq = _mm_div_ps(freq, freq_per_bin);

					freq = _mm_mul_ps(two_pi, freq);
					freq = _mm_div_ps(freq, pitch_quality);

					__m128 overlap_phase = _mm_set_ps((float)(k * 4 + 3) * CUTE_SOUND_EXPECTED_FREQUENCY, (float)(k * 4 + 2) * CUTE_SOUND_EXPECTED_FREQUENCY, (float)(k * 4 + 1) * CUTE_SOUND_EXPECTED_FREQUENCY, (float)(k * 4) * CUTE_SOUND_EXPECTED_FREQUENCY);
					freq = _mm_add_ps(freq, overlap_phase);

					__m128 phase = sum_phase[k];
					phase = _mm_add_ps(phase, freq);
					sum_phase[k] = phase;

					__m128 c, s;
					_mm_sincos_ps(phase, &s, &c);
					__m128 real = _mm_mul_ps(mag, c);
					__m128 imag = _mm_mul_ps(mag, s);

					fft_data[k] = real;
					fft_data[(CUTE_SOUND_PITCH_FRAME_SIZE / 4) + k] = imag;
				}
			}

			for (int k = CUTE_SOUND_PITCH_FRAME_SIZE + 2; k < 2 * CUTE_SOUND_PITCH_FRAME_SIZE - 2; ++k)
				pf->fft_data[k] = 0;

			cs_fft(pf->fft_data, pf->fft_data + CUTE_SOUND_PITCH_FRAME_SIZE, CUTE_SOUND_PITCH_FRAME_SIZE, -1);

			{
				__m128* fft_data = (__m128*)pf->fft_data;
				__m128* window_accumulator = (__m128*)pf->window_accumulator;

				for (int k = 0; k < CUTE_SOUND_PITCH_FRAME_SIZE / 4; ++k)
				{
					__m128 von_hann = cs_vonhann4(k);
					__m128 fft_data_segment = fft_data[k];
					__m128 accumulator_segment = window_accumulator[k];
					__m128 divisor = _mm_div_ps(pitch_quality, _mm_set_ps1(8.0f));
					fft_data_segment = _mm_mul_ps(von_hann, fft_data_segment);
					fft_data_segment = _mm_div_ps(fft_data_segment, divisor);
					accumulator_segment = _mm_add_ps(accumulator_segment, fft_data_segment);
					window_accumulator[k] = accumulator_segment;
				}
			}

			memcpy(pf->out_FIFO, pf->window_accumulator, CUTE_SOUND_STEPSIZE * sizeof(float));
			memmove(pf->window_accumulator, pf->window_accumulator + CUTE_SOUND_STEPSIZE, CUTE_SOUND_PITCH_FRAME_SIZE * sizeof(float));
			memmove(pf->in_FIFO, pf->in_FIFO + CUTE_SOUND_STEPSIZE, CUTE_SOUND_OVERLAP * sizeof(float));
		}
	}
}


/*
	------------------------------------------------------------------------------
	This software is available under 2 licenses - you may choose the one you like.
	------------------------------------------------------------------------------
	ALTERNATIVE A - zlib license
	Copyright (c) 2017 Randy Gaul http://www.randygaul.net
	This software is provided 'as-is', without any express or implied warranty.
	In no event will the authors be held liable for any damages arising from
	the use of this software.
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not
		 be misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.
	------------------------------------------------------------------------------
	ALTERNATIVE B - Public Domain (www.unlicense.org)
	This is free and unencumbered software released into the public domain.
	Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
	software, either in source code form or as a compiled binary, for any purpose,
	commercial or non-commercial, and by any means.
	In jurisdictions that recognize copyright laws, the author or authors of this
	software dedicate any and all copyright interest in the software to the public
	domain. We make this dedication for the benefit of the public at large and to
	the detriment of our heirs and successors. We intend this dedication to be an
	overt act of relinquishment in perpetuity of all present and future rights to
	this software under copyright law.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
	ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	------------------------------------------------------------------------------
*/