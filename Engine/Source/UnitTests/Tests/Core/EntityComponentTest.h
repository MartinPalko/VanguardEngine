#include "gtest/gtest.h"
#include "World.h"

#include "Entity\Transform.h"

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

		//Component* newTransform = testEntity->AddComponent("Transform");
		Component* newTransform = testEntity->AddComponent<Transform>();
		EXPECT_NE(newTransform, nullptr);

		Component* newInvalidType = testEntity->AddComponent("Entity");
		EXPECT_EQ(newInvalidType, nullptr);

		delete testEntity;
	}
}
