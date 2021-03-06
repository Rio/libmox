/*
 *  Codec.cpp
 *  MoxFiles
 *
 *  Created by Brendan Bolles on 4/22/15.
 *  Copyright 2015 fnord. All rights reserved.
 *
 */

#include <MoxFiles/Codec.h>

#include <MoxFiles/UncompressedCodec.h>
#include <MoxFiles/MPEGCodec.h>

#include <MoxFiles/UncompressedPCMCodec.h>


namespace MoxFiles
{

Box2i
VideoCodec::dataWindow() const
{
	const MoxMxf::VideoDescriptor *descriptor = getDescriptor();
	
	if(descriptor == NULL)
		throw MoxMxf::NullExc("NULL descriptor");
	
	const UInt32 stored_width = descriptor->getStoredWidth();
	const UInt32 stored_height = descriptor->getStoredHeight();
	
	const Int32 x_offset = descriptor->getSampledXOffset() + descriptor->getDisplayXOffset();
	const Int32 y_offset = descriptor->getSampledYOffset() + descriptor->getDisplayYOffset();
	
	assert(descriptor->getSampledXOffset() == 0 && descriptor->getSampledYOffset() == 0);
	
	
	Box2i dw;
	
	dw.min.x = -x_offset;
	dw.min.y = -y_offset;
	
	dw.max.x = (stored_width - x_offset) - 1;
	dw.max.y = (stored_height - y_offset) - 1;
	
	return dw;
}


Box2i
VideoCodec::displayWindow() const
{
	const MoxMxf::VideoDescriptor *descriptor = getDescriptor();
	
	if(descriptor == NULL)
		throw MoxMxf::NullExc("NULL descriptor");
	
	const UInt32 display_width = descriptor->getDisplayWidth();
	const UInt32 display_height = descriptor->getDisplayHeight();
	
	
	Box2i dw;
	
	dw.min.x = 0;
	dw.min.y = 0;
	
	dw.max.x = display_width - 1;
	dw.max.y = display_height - 1;
	
	return dw;
}


Box2i
VideoCodec::sampledWindow() const
{
	const MoxMxf::VideoDescriptor *descriptor = getDescriptor();
	
	if(descriptor == NULL)
		throw MoxMxf::NullExc("NULL descriptor");
	
	const UInt32 stored_width = descriptor->getStoredWidth();
	const UInt32 stored_height = descriptor->getStoredHeight();
	
	const Int32 x_offset = descriptor->getSampledXOffset();
	const Int32 y_offset = descriptor->getSampledYOffset();
	
	assert(descriptor->getSampledXOffset() == 0 && descriptor->getSampledYOffset() == 0);
	
	
	Box2i dw;
	
	dw.min.x = -x_offset;
	dw.min.y = -y_offset;
	
	dw.max.x = (stored_width - x_offset) - 1;
	dw.max.y = (stored_height - y_offset) - 1;
	
	return dw;
}


DataChunkPtr
VideoCodec::getNextData()
{
	DataChunkPtr dat;
	
	if(!_data_queue.empty())
	{
		dat = _data_queue.front();
		
		_data_queue.pop();
	}
	
	return dat;
}


FrameBufferPtr
VideoCodec::getNextFrame()
{
	FrameBufferPtr frm;
	
	if(!_frame_queue.empty())
	{
		frm = _frame_queue.front();
		
		_frame_queue.pop();
	}
	
	return frm;
}


void
VideoCodec::storeData(DataChunkPtr dat)
{
	_data_queue.push(dat);
}


void
VideoCodec::storeFrame(FrameBufferPtr frm)
{
	_frame_queue.push(frm);
}


PixelType
VideoCodecInfo::compressedType(PixelType pixelType) const
{
	if(pixelType == MoxFiles::UINT32)
	{
		if( !canCompressType(MoxFiles::UINT32) )
			throw MoxMxf::LogicExc("Trying to push ID channel to incompatible codec");
	}

	// rounding up
	switch(pixelType)
	{
		case MoxFiles::UINT8:
			if( canCompressType(MoxFiles::UINT8) )
				return MoxFiles::UINT8;
		
		case MoxFiles::UINT10:
			if( canCompressType(MoxFiles::UINT10) )
				return MoxFiles::UINT10;

		case MoxFiles::UINT12:
			if( canCompressType(MoxFiles::UINT12) )
				return MoxFiles::UINT12;
		
		case MoxFiles::UINT16:
		case MoxFiles::UINT16A:
			if( canCompressType(MoxFiles::UINT16) )
				return MoxFiles::UINT16;
		
		case MoxFiles::HALF:
			if( canCompressType(MoxFiles::HALF) )
				return MoxFiles::HALF;
		
		case MoxFiles::FLOAT:
			if( canCompressType(MoxFiles::FLOAT) )
				return MoxFiles::FLOAT;
	}
	
	// if we got here, unable to round up to higher depth, so must round down
	switch(pixelType)
	{
		case MoxFiles::FLOAT:
			if( canCompressType(MoxFiles::FLOAT) )
				return MoxFiles::FLOAT;
		
		case MoxFiles::HALF:
			if( canCompressType(MoxFiles::HALF) )
				return MoxFiles::HALF;
				
		case MoxFiles::UINT16:
		case MoxFiles::UINT16A:
			if( canCompressType(MoxFiles::UINT16) )
				return MoxFiles::UINT16;
				
		case MoxFiles::UINT12:
			if( canCompressType(MoxFiles::UINT12) )
				return MoxFiles::UINT12;
				
		case MoxFiles::UINT10:
			if( canCompressType(MoxFiles::UINT10) )
				return MoxFiles::UINT10;
				
		case MoxFiles::UINT8:
			if( canCompressType(MoxFiles::UINT8) )
				return MoxFiles::UINT8;
	}
	
	throw MoxMxf::LogicExc("Unable to find a compressed type");
}


SampleType
AudioCodecInfo::compressedType(SampleType sampleType) const
{
	// round up
	switch(sampleType)
	{
		case MoxFiles::UNSIGNED8:
			if( canCompressType(MoxFiles::UNSIGNED8) )
				return MoxFiles::UNSIGNED8;
		
		case MoxFiles::SIGNED16:
			if( canCompressType(MoxFiles::SIGNED16) )
				return MoxFiles::SIGNED16;
		
		case MoxFiles::SIGNED24:
			if( canCompressType(MoxFiles::SIGNED24) )
				return MoxFiles::SIGNED24;
		
		case MoxFiles::SIGNED32:
			if( canCompressType(MoxFiles::SIGNED32) )
				return MoxFiles::SIGNED32;
		
		case MoxFiles::AFLOAT:
			if( canCompressType(MoxFiles::AFLOAT) )
				return MoxFiles::AFLOAT;
	}
	
	// no dice, round down
	switch(sampleType)
	{
		case MoxFiles::AFLOAT:
			if( canCompressType(MoxFiles::AFLOAT) )
				return MoxFiles::AFLOAT;
				
		case MoxFiles::SIGNED32:
			if( canCompressType(MoxFiles::SIGNED32) )
				return MoxFiles::SIGNED32;
				
		case MoxFiles::SIGNED24:
			if( canCompressType(MoxFiles::SIGNED24) )
				return MoxFiles::SIGNED24;
				
		case MoxFiles::SIGNED16:
			if( canCompressType(MoxFiles::SIGNED16) )
				return MoxFiles::SIGNED16;
				
		case MoxFiles::UNSIGNED8:
			if( canCompressType(MoxFiles::UNSIGNED8) )
				return MoxFiles::UNSIGNED8;
	}
	
	throw MoxMxf::LogicExc("Unable to find a compressed type");
}


typedef std::map<VideoCompression, VideoCodecInfo *> VideoCodecList;
	
class VideoCodecListStorage
{
  public:
	VideoCodecListStorage() {}
	~VideoCodecListStorage();
	
	VideoCodecList & getCodecList() { return _video_codec_list; }

  private:
	VideoCodecList _video_codec_list;
};

VideoCodecListStorage::~VideoCodecListStorage()
{
	for(VideoCodecList::iterator i = _video_codec_list.begin(); i != _video_codec_list.end(); ++i)
	{
		delete i->second;
	}
}


const VideoCodecInfo &
getVideoCodecInfo(VideoCompression videoCompression)
{
	static VideoCodecListStorage codecListStorage;
	
	VideoCodecList &codecList = codecListStorage.getCodecList();
	
	if( codecList.empty() )
	{
		codecList[UNCOMPRESSED] = new UncompressedCodecInfo;
		codecList[MPEG] = new MPEGCodecInfo;
	}
	
	if(codecList.find(videoCompression) == codecList.end())
		throw MoxMxf::ArgExc("Requested VideoCodecInfo for unknown compression.");
	
	return *codecList[videoCompression];
}


const VideoCodecInfo &
getVideoCodecInfo(MoxMxf::VideoDescriptor::VideoCodec codec)
{
	if(codec == MoxMxf::VideoDescriptor::VideoCodecUncompressedRGB)
	{
		return getVideoCodecInfo(UNCOMPRESSED);
	}
	else if(codec == MoxMxf::VideoDescriptor::VideoCodecMPEG2)
	{
		return getVideoCodecInfo(MPEG);
	}
	
	throw MoxMxf::InputExc("Unknown video codec");
}


DataChunkPtr
AudioCodec::getNextData()
{
	DataChunkPtr dat;
	
	if(!_data_queue.empty())
	{
		dat = _data_queue.front();
		
		_data_queue.pop();
	}
	
	return dat;
}


AudioBufferPtr
AudioCodec::getNextBuffer()
{
	AudioBufferPtr buf;
	
	if(!_audio_queue.empty())
	{
		buf = _audio_queue.front();
		
		_audio_queue.pop();
	}
	
	return buf;
}


void
AudioCodec::storeData(DataChunkPtr dat)
{
	_data_queue.push(dat);
}


void
AudioCodec::storeBuffer(AudioBufferPtr buf)
{
	_audio_queue.push(buf);
}


typedef std::map<AudioCompression, AudioCodecInfo *> AudioCodecList;
	
class AudioCodecListStorage
{
  public:
	AudioCodecListStorage() {}
	~AudioCodecListStorage();
	
	AudioCodecList & getCodecList() { return _audio_codec_list; }

  private:
	AudioCodecList _audio_codec_list;
};

AudioCodecListStorage::~AudioCodecListStorage()
{
	for(AudioCodecList::iterator i = _audio_codec_list.begin(); i != _audio_codec_list.end(); ++i)
	{
		delete i->second;
	}
}


const AudioCodecInfo &
getAudioCodecInfo(AudioCompression audioCompression)
{
	static AudioCodecListStorage codecListStorage;
	
	AudioCodecList &codecList = codecListStorage.getCodecList();
	
	if( codecList.empty() )
	{
		codecList[PCM] = new UncompressedPCMCodecInfo;
	}
	
	if(codecList.find(audioCompression) == codecList.end())
		throw MoxMxf::ArgExc("Requested AudioCodecInfo for unknown compression.");
	
	return *codecList[audioCompression];
}


const AudioCodecInfo &
getAudioCodecInfo(MoxMxf::AudioDescriptor::AudioCodec codec)
{
	if(codec == MoxMxf::AudioDescriptor::AudioCodecUncompressedPCM ||
		codec == MoxMxf::AudioDescriptor::AudioCodecAES3)
	{
		return getAudioCodecInfo(PCM);
	}
	
	throw MoxMxf::InputExc("Unknown video codec");
}


} // namespace
