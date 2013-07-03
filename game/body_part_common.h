#pragma once

namespace am {
namespace game {

	class BodyPartType {
	public:

		enum PartType {
			UNKNOWN_PART, NONE, ANY, HAND, ARM, LEG, HEAD, TORSO, NECK, SHOULDERS, LEGS, FEET, MAX_BODY_TYPE_LENGTH
		};

		static const char *getNiceBodyPartName(PartType type);
		static const char *getBodyPartName(PartType type);
		static const char *getBodyPartName(int type);

		static PartType getBodyPartType(int typeValue);
		static PartType getBodyPartTypeFromNice(const char *typeName);
		static PartType getBodyPartType(const char *typeName);

	protected:

		static const char *sNiceBodyPartTypeNames[];
		static const char *sBodyPartTypeNames[];

	};

}
}
