// TASK:
//
// Complete the `delivery_cost` function.

pub enum UserSubscription {
    /// The user is not subscribed.
    NotSubscribed,
    /// The user has been subscribed for a given number of months.
    Subscribed { months: u32 },
}

/// Calculates the delivery cost for an order.
///
/// The delivery cost is calculated differently depending on the user's subscription status.
///
/// The base cost is 50.
///
/// An un-subscribed user gets free delivery if the order cost is 1000 or more.
/// A subscribed user gets free delivery if the order cost is 500 or more.
/// If the user has been subscribed for 12 months or more, they get free delivery regardless of the order cost.
pub fn delivery_cost(subscription: UserSubscription, order_cost: u32) -> u32 {
    use UserSubscription::*;
    return match subscription {
        NotSubscribed if order_cost >= 1000 => {
            0
        },
        Subscribed { months } if order_cost >= 500 || months >= 12 => 0,
        _ => 50,
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_delivery_cost() {
        assert_eq!(delivery_cost(UserSubscription::NotSubscribed, 1000), 0);
        assert_eq!(delivery_cost(UserSubscription::NotSubscribed, 999), 50);
        assert_eq!(delivery_cost(UserSubscription::Subscribed { months: 11 }, 500), 0);
        assert_eq!(delivery_cost(UserSubscription::Subscribed { months: 11 }, 499), 50);
        assert_eq!(delivery_cost(UserSubscription::Subscribed { months: 12 }, 500), 0);
        assert_eq!(delivery_cost(UserSubscription::Subscribed { months: 12 }, 499), 0);
    }
}