#include "notificationServiceTest.h"

TEST(NotificationServiceTest, GetInstance){
	NotificationService& notificationService1 = NotificationService::getInstance();
	NotificationService& notificationService2 = NotificationService::getInstance();

	EXPECT_NE(&notificationService1, nullptr);
	EXPECT_EQ(&notificationService1, &notificationService2);
}

TEST(NotificationServiceTest, NotifyOnReshapeWindow) {
	NotificationService& notificationService = NotificationService::getInstance();
	MockCamera* mockCamera1{new MockCamera()};
	MockCamera* mockCamera2{new MockCamera()};

	notificationService.subscribeListener(mockCamera1);
	notificationService.subscribeListener(mockCamera2);

	EXPECT_CALL(*mockCamera1, onWindowReshape)
		.Times(1);
	EXPECT_CALL(*mockCamera2, onWindowReshape)
		.Times(1);
	notificationService.notifyOnReshapeWindow(1, 1);

	notificationService.unsubscribeListener(mockCamera1);
	notificationService.unsubscribeListener(mockCamera2);

	EXPECT_CALL(*mockCamera1, onWindowReshape)
		.Times(0);
	EXPECT_CALL(*mockCamera2, onWindowReshape)
		.Times(0);
	notificationService.notifyOnReshapeWindow(1, 1);

	delete mockCamera1;
	delete mockCamera2;
}