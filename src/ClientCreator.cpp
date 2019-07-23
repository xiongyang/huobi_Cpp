#include "Huobi/SubscriptionClient.h"
#include "SubscriptionClientImpl.h"
#include "SyncClientImpl.h"
#include "Huobi/SubscriptionOptions.h"
#include "Huobi/RequestOptions.h"
#include "Huobi/QtSubscribeClient.h"
#include "QtSubscribeClientImpl.h"
#include "Huobi/QtRestClient.h"
#include "QtRestClientImpl.h"

namespace Huobi {

    SubscriptionClient *createSubscriptionClient(const char *apiKey, const char *secretKey, SubscriptionOptions &op) {
        return new SubscriptionClientImpl(apiKey, secretKey, op);
    }

    SubscriptionClient *createSubscriptionClient() {
        return new SubscriptionClientImpl();
    }

    SubscribeClient *createSubscribeClient() {
        return new QtSubscribeClientImpl();
    }


    SubscribeClient *createSubscribeClient(const char *accessKey, const char *secretKey) {
        return new QtSubscribeClientImpl(accessKey, secretKey);
    }

    SubscriptionClient *createSubscriptionClient(const char *apiKey, const char *secretKey) {
        SubscriptionOptions op;
        return new SubscriptionClientImpl(apiKey, secretKey, op);
    }

    SubscriptionClient *createSubscriptionClient(SubscriptionOptions &op) {
        return new SubscriptionClientImpl(op);
    }

    RequestClient *createRequestClient() {
        return new SyncClientImpl();
    }

    RestClient *createRestClient() {
        return new QtRestClientImpl();
    }

    RestClient *createRestClient(const char *apiKey, const char *secretKey) {
        return new QtRestClientImpl(apiKey, secretKey);
    }

    RequestClient *createRequestClient(const char *apiKey, const char *secretKey) {
        return new SyncClientImpl(apiKey, secretKey);
    }

    RequestClient *createRequestClient(RequestOptions &op) {
        return new SyncClientImpl(op);
    }

    RequestClient *createRequestClient(const char *apiKey, const char *secretKey, RequestOptions &op) {
        return new SyncClientImpl(apiKey, secretKey, op);
    }
}