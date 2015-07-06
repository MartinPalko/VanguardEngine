#include "gtest/gtest.h"
#include "EntityComponentSystem.h"

namespace Vanguard
{

	class EntityComponentTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(EntityComponentTest, AddingComponents)
	{
		Entity* testEntity = new Entity();

		Component* newTransform = testEntity->AddComponent(INativeClassInfo::GetType("Transform"));
		EXPECT_NE(newTransform, nullptr);

		Component* newInvalidType = testEntity->AddComponent(INativeClassInfo::GetType("Entity"));
		EXPECT_EQ(newInvalidType, nullptr);

		delete testEntity;
	}
}
