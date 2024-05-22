#include <assert.h>

#include "deck.h"
#include "move.h"

struct deck_t {
  struct tile_t* deck;   // The deck we are playing with
  size_t size;           // The size of the deck
  size_t capacity;       // The capacity of the deck
};

struct deck_t* deck_empty(size_t capacity) {
  struct deck_t* res = malloc(sizeof(*res));
  res->capacity = capacity;
  res->size = 0;
  res->deck = malloc(sizeof(*res->deck) * res->capacity);
  return res;
}

void deck_add(struct deck_t* d, struct tile_t t) {
  assert(d->size < d->capacity);
  d->deck[d->size] = t;
  d->size++;
}

size_t deck_size(const struct deck_t* d) {
  return d->size;
}

struct tile_t deck_get(const struct deck_t* d, unsigned int idx) {
  if (idx >= d->size)
    return CARC_TILE_EMPTY;
  return d->deck[idx];
}

struct deck_t* deck_copy(const struct deck_t* d) {
  struct deck_t* e = deck_empty(d->capacity);
  e->size = d->size;
  for (unsigned int i=0; i<e->size; i++) {
    e->deck[i] = d->deck[i];
  }
  return e;
}

void deck_free(struct deck_t* d) {
  free(d->deck);
  free(d);
}
