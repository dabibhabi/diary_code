package store

import (
	"errors"
	"sync"
	"time"

	"simple_backend/internal/models"
)

var ErrNotFound = errors.New("not found")

type Store interface {
	List() []models.Item
	Get(id string) (models.Item, error)
	Create(name string) models.Item
	Delete(id string) error
}

type MemoryStore struct {
	mu    sync.RWMutex
	items map[string]models.Item
	seq   int
}

func NewMemoryStore() *MemoryStore {return &MemoryStore{items: map[string]models.Item{}}}

func (m *MemoryStore) List() []models.Item {
	m.mu.RLock()
	defer m.mu.RUnlock()
	out := make([]models.Item, 0, len(m.items))
	for _, it := range m.items {
		out = append(out, it)
	}
	return out
}

func (m *MemoryStore) Get(id string) (models.Item, error) {
	m.mu.RLock()
	defer m.mu.RUnlock()
	it, ok := m.items[id]
	if !ok {
		return models.Item{}, ErrNotFound
	}
	return it, nil
}

func (m *MemoryStore) Create(name string) models.Item {
	m.mu.Lock()
	defer m.mu.Unlock()
	m.seq++
	it := models.Item{
		ID:        itoa(m.seq),
		Name:      name,
		CreatedAt: time.Now().UTC(),
	}
	m.items[it.ID] = it
	return it
}

func (m *MemoryStore) Delete(id string) error {
	m.mu.Lock()
	defer m.mu.Unlock()
	if _, ok := m.items[id]; !ok {return ErrNotFound}
	delete(m.items, id)
	return nil
}

func itoa(n int) string {
	if n == 0 { return "0" }
	var buf [20]byte
	i := len(buf)
	for n > 0 {
		i--
		buf[i] = byte('0' + n%10)
		n /= 10
	}
	return string(buf[i:])
}
