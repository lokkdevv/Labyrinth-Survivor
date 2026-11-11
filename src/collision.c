#include "../include/collision.h"

void BBcollide(CollisionRec *rec1, CollisionRec *rec2) {
    if (!CheckCollisionRecs(rec1->rec, rec2->rec))
        return;
    if (rec1->movable && rec2->movable)
        return;
    Rectangle collision_rec = GetCollisionRec(rec1->rec, rec2->rec);
    if (collision_rec.width < collision_rec.height) {
        if (rec1->movable && !rec2->movable) {
            if (rec1->rec.x < rec2->rec.x)
                rec1->rec.x -= collision_rec.width;
            else
                rec1->rec.x += collision_rec.width;
        } else if (rec2->movable && !rec1->movable) {
            if (rec2->rec.x < rec1->rec.x)
                rec2->rec.x -= collision_rec.width;
            else
                rec2->rec.x += collision_rec.width;
        }
    } else {
        if (rec1->movable && !rec2->movable) {
            if (rec1->rec.y < rec2->rec.y)
                rec1->rec.y -= collision_rec.height;
            else
                rec1->rec.y += collision_rec.height;
        } else if (rec2->movable && !rec1->movable) {
            if (rec2->rec.y < rec1->rec.y)
                rec2->rec.y -= collision_rec.height;
            else
                rec2->rec.y += collision_rec.height;
        }
    }
}


void BCcollide(CollisionRec rect, CollisionCircle *circle) {
    Vector2 center = {
        circle->circle.x,
        circle->circle.y
    };

    float radius = circle->circle.radius;

    // Check if a collision exists
    if (!CheckCollisionCircleRec(center, radius, rect.rec))
        return;

    // Find the closest point on the rectangle to the circle
    float closestX = clamp(
        center.x,
        rect.rec.x,
        rect.rec.x + rect.rec.width
    );

    float closestY = clamp(
        center.y,
        rect.rec.y,
        rect.rec.y + rect.rec.height
    );

    // Calculate direction from rectangle to circle
    float dx = center.x - closestX;
    float dy = center.y - closestY;

    float distance = sqrtf(dx * dx + dy * dy);

    // Handle circle center inside rectangle
    if (distance == 0.0f) {
        float left   = center.x - rect.rec.x;
        float right  = rect.rec.x + rect.rec.width - center.x;
        float top    = center.y - rect.rec.y;
        float bottom = rect.rec.y + rect.rec.height - center.y;

        float min = minf(fminf(left, right), fminf(top, bottom));

        if (min == left)
            circle->circle.x -= radius + left;
        else if (min == right)
            circle->circle.x += radius + right;
        else if (min == top)
            circle->circle.y -= radius + top;
        else
            circle->circle.y += radius + bottom;

        return;
    }

    // Calculate how much the circle overlaps the rectangle
    float overlap = radius - distance;

    // Normalize direction and push circle outside rectangle
    circle->circle.x += (dx / distance) * overlap;
    circle->circle.y += (dy / distance) * overlap;
}
